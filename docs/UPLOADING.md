# Firmware upload methods
Current machines allow uploading/updating firmware. Goals are:
 - Customers should be able to upload firmware through USB from a normal
   computer, without needing any specific hardware (i.e. SWD/ISP/JTAG programmer).
 - The regular USB upload method should be resilient, it should be impossible
   to let it break itself by uploading the wrong firmware (or random binary data).
   Of course USB uploads might become broken when using SWD/ISP/JTAG.

### Filament maker
 - The filament maker has an **AVR ATmega2560-based mainboard** (called devoboard).
 - The board contains a **secondary ATmega16u2** that connects to the USB plug and has a UART connection to the 2560. Soon after the board is assembled, this 16u2 is flashed (through ISP) with firmware that makes it **behave like an USB-to-serial converter** (e.g. like FTDI chips). At the same time, its fuse and lock bits are set. The firmware on the 16u2 is static and not expected to be updated ever (though I believe it *does* implement a DFU mode that could be used to update through USB if ever needed, but that is fragile in that it could break further USB updates if it goes wrong).
 - Soon after the board is assembled, the **2560 is flashed with a bootloader** (optiboot), and its fuse and lock bits are set. This bootloader handles uploading regular firmware through USB by talking a standard ("STK500"-ish) protocol over the UART (which is forwarded to USB by the 16u2) and then self-programming its own flash. This bootloader is static and not expected to be updated (updating needs an ISP programmer). Source of the bootloader is in [(our fork of the) optiboot repository](https://github.com/3devo/optiboot).
 - **The board info** (e.g. PCB revision and other info) is stored inside the bootloader flash area and uploaded as part of the bootloader. More details about the board info [can be found in the CommonFirmware repository](https://github.com/3devo/CommonFirmware/blob/dev/doc/Boardinfo.md).
 - **The bootloader lives in a special section of flash**. In the fuse bytes, the reset vector of the MCU is configured to jump to the bootloader first. The lock bytes are configured so the bootloader section cannot be written to (except when using ISP externally), to prevent the bootloader from overwriting itself.
 - **Firmware uploads are done using the `avrdude` command** as shipped with Arduino. Development uploads are done by the Arduino tooling which essentially just calls `avrdude` with the proper arguments (protocol, serial port, baud rate, hex file to upload) to handle the actual upload.
 - **Firmware uploads work as follows**:
	 - `avrdude` first opens the serial port and a handshaking signal is set (this is probably done automatically by the kernel). This toggles a pin on the 16u2 which (through an RC filter) causes a short pulse on the 2560 reset line, resetting the 2560.
	 - This reset causes the bootloader to be started, which (if it sees it was started due to the reset pin) listens for a short on the UART for a command (sent by `avrdude`). On other resets (e.g. power-on reset or watchdog reset or so), the bootloader just jumps straight into the main firmware.
	 - If a command is received, the bootloader stays running and handles the firmware upload.
	 - If no command is received within a short timeout, the bootloader starts the main firmware.
 - The firmware upload mechanism (both hardware and software) is essentially a direct copy of what the Arduino 2560 uses.
 - To manually upload firmware:
   ```
   avrdude -v -V -patmega2560 -carduino -P/dev/ttyACM0 -b115200 -D -Uflash:w:FeFirmware.ino.hex:i
   ```

### GP20 (shredder/granulator) / AD17 (Airid)
 - These machines both have an **STM32F4-based mainboard**.
 - **The main MCU is directly connected to USB**, without an intermediate USB-to-serial. This means in normal operation, the main MCU and firmware are responsible for attaching to USB.
 - Soon after the board is assembled, the **STM32 is flashed with a "Bootstrap" sketch**. This is a sketch that flashes the relevant board info (e.g. PCB version) into the on-chip STM32 area. After this is done, the Bootstrap sketch serves no further purpose and it is replaced by a regular firmware next. More details about the board info [can be found in the CommonFirmware repository](https://github.com/3devo/CommonFirmware/blob/dev/doc/Boardinfo.md). Source for the Bootstrap sketch can be found in [the STM32-Bootstrap-firmware repository](https://github.com/3devo/STM32-Bootstrap-firmware).
 - **Firmware uploads are done using the `dfu-util` command** installed system-wide. Development uploads are done by the Arduino tooling which essentially just calls `dfu-util` with the proper arguments (USB id's, hex file to upload) to handle the actual upload.
 - **Firmware uploads are handled by the built-in STM32 "system" bootloader**. This is a simple bootloader that lives in ROM and supports the USB DFU protocol for firmware uploads.
 - **Regular firmware uploads work as follows**:
	 - The main firmware advertises as a DFU "runtime device". This is a device that essentially has one command: Switch into firmware upload (DFU) mode.
	 - `dfu-util` is passed both the runtime and DFU mode USB identifiers. When it sees the runtime identifiers, it sends a command to switch to DFU mode. If it finds the device already in DFU mode, it just starts the upload directly.
	 - In response to the switch-mode command, the main firmware sets some registers, forces a reset and then after the reset jumps into the system bootloader.
	 - The system bootloader advertises as a regular DFU device, waiting for DFU commands.
	 - `dfu-util` communicates with the system bootloader to handle the firmware upload.
	 - After the firmware is uploaded, `dfu-util` sends a reset command (this is an ST-specific add-on, I think) which resets into the main firmware again.
 - In addition to USB DFU uploads, the **system bootloader also supports uploads through SPI, UART, etc.** It is not entirely clear how the bootloader decides what channel to use ([system bootloader source is not available](https://community.st.com/s/idea/0873W000000PAwtQAG/detail)), but care has been taking in the board design (with e.g. pullups and pinout choices) to ensure that the other channels are kept idle when the bootloader runs, so it will not be tempted to activate any of the other channels.
 - The way the **main firmware handles reset into the bootloader** is not currently (2023-03) implemented in the STM32 Arduino core. There has been [a PR for this](https://github.com/stm32duino/Arduino_Core_STM32/pull/710) since 2019, but it was never finalized. We have built on top of this PR to get it into shape to work for our usecase (also adding the runtime DFU stuff) and shared our code, but finalizing this PR is stalled (the maintainer is aware of this - just too much to do). This also means that we are still running on a fork of a fairly old STM32 core, but it works well enough for now.
 - To manually upload a firmware:
   ```
   dfu-util --device 0x16d0:0x102A,0x0483:0xdf11 --alt 0 --dfuse-address :leave -v -D /path/to/update.dfuse
   ```

### 3devo update
 - This is a tool, written in .NET, running on Windows only by 3devo (based on an existing program) that handles firmware uploads using a friendly GUI usable by customers to update their machines.
 - Source for this tool is in [the 3devoUpdate repository](https://github.com/3devo/3devoUpdate)
 - The tool automatically detects serial ports and USB devices and, based on the USB identifiers, allows the user a choice of connected 3devo devices.
 - The user can select a firmware file to upload, and the tool handles running the (bundled) `avrude` or `dfu-util` commands
 - For the STM32-based machines, a .dfuse file is used. In addition to the actual firmware, these files also contain the target USB identifiers, which are checked by 3devo update to prevent mixing up firmware versions (e.g. uploading Airid firmware to GP20). Protection is limited, though, since there are no (cryptographic) signatures involved. For the devoboard, a plain .hex (intel hex) file is used that has no such indication, so a user could upload any hex file and break things.
 - For uploads under other OS's, the tool has a README with instructions how to run `avrdude` or `dfu-util` manually.
