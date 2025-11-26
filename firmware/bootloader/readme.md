# Bootloader

See https://wiki.rusefi.com/Firmware-update-via-CAN

Here we combine ChibiOS with OpenBLT

Q: how do I build bootloader for specific board say proteus/meta-info-proteus_f7.env?

A:  
Option 1:
```
./firmware/bin/compile.sh config/boards/proteus/meta-info-proteus_f7.env bootloader/blbuild/openblt_proteus_f7.bin
```
Option 2:
```
cd firmware/bootloader
source ../config/boards/common_script_read_meta_env.inc ../config/boards/proteus/meta-info-proteus_f7.env
make -j$(nproc)
```
