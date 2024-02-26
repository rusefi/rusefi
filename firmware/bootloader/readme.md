# Bootloader

See https://github.com/rusefi/rusefi/wiki/Firmware-update-via-CAN

Here we combine ChibiOS with OpenBLT

Q: how do I build bootloader for specific board say proteus/meta-info-proteus_f7.env?

A:
```
cd firmware/bootloader
source ../config/boards/common_script_read_meta_env.inc ../config/boards/hellen/small-can-board
make
```
