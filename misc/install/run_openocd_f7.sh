#!/bin/bash

STLINK=$(lsusb | grep 'ST-LINK\|STLINK')
if [ "$STLINK" ]; then
  echo "I see $STLINK make sure your board file accepts this specific kind of ST-LINK"
else
  echo "Do not see any ST-LINK. If you are on WSL make sure you have bound USB"
  exit -1
fi

sudo openocd -f board/st_nucleo_f7.cfg -c init -c reset
