#!/bin/bash

echo "I am flash_openocd.sh"

cd ../misc/install

[ -e openocd/openocd.exe ] || { echo "openocd/openocd.exe NOT FOUND"; exit 1; }

./openocd/openocd.exe -f openocd/stm32f4discovery.cfg  -c "program ../../firmware/build/rusefi.bin verify reset exit 0x08000000"

