#!/bin/sh

# STM32F469 version of the firmware for https://rusefi.com/forum/viewtopic.php?f=4&t=1215

cd ../../..
export PROJECT_BOARD=prometheus/f469
export USE_BOOTLOADER=yes

sh config/boards/common_make.sh

