3Devo platform and board files for Arduino
==========================================
These files allow using the Devoboard in the Arduino IDE, allowing
access to all pins on the board. To install, make a clone if this
repository into your Sketchbook/hardware folder (so you would have
`Sketchbook/hardware/3devo/avr/platform.txt`, for example).

### Udev rules for USB device access (Linux)
To allow accessing USB devices, udev rules are needed to hand out access to
normal users (resist the temptation to just run under sudo, since that might
end up creating root-owned files everywhere and prevent running as non-root
later or cause other issues).

For most devices, such rules are installed along with software (e.g. `apt
install dfu-util avrdude openocd` gives you most relevant rules), but our own
boards have custom USB identifiers unknown to these tools, so this needs
[a custom rule file](udev-rules/3devo-devices.rules).

To install it, do something like:

```
sudo cp ./udev-rules/3devo-devices.rules /etc/udev/rules.d
```

These rules should take effect directly when replugging the device, no restart
or reload required.

In addition, some devices hand out access to the `plugdev` or `dialout` groups,
so it is useful to add your own user to those groups:

```
sudo adduser yourusername dialout plugdev
```

This applies to Linux, under Windows permissions to USB devices are probably
handed by default.
