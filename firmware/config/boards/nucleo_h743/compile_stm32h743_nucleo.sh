#!/bin/bash

# STM32H743 version of the firmware for Nucleo-H743 board

SCRIPT_NAME="compile_nucleo_h743.sh"
echo "Entering $SCRIPT_NAME"

bash ../common_make.sh nucleo_h743 ARCH_STM32H7
