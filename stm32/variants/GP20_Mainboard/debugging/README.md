Debugging with OpenOCD
======================
This directory contains config files to debug with Gdb and OpenOCD. To
use these, simply start gdb, passing the gdb config and the .elf you
want to debug:

    arm-none-eabi-gdb -x gdb.init /path/to/Sketch.ino.elf

This assumes that the code is already uploaded to the board through
other means (though it should be possible to upload through OpenOCD,
maybe also gdb if needed).
