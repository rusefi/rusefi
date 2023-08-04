#!/bin/bash

echo "I am flash_openocd407.sh"

STLINK=$(lsusb | grep 'ST-LINK\|STLINK')
if [ "$STLINK" ]; then
  echo "I see $STLINK make sure your board file accepts this specific kind of ST-LINK"
else
  echo "Do not see any ST-LINK. If you are on WSL make sure you have bound USB"
  exit -1
fi

sudo openocd -f board/stm32f4discovery.cfg  -c "program build/rusefi.bin verify reset exit 0x08000000"
