#!/bin/bash

echo "I am flash_openocd.sh"


openocd -f board/stm32f4discovery.cfg  -c "program ../../firmware/build/rusefi.bin verify reset exit 0x08000000"

