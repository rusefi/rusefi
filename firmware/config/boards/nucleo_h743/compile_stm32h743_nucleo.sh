#!/bin/bash

# STM32H743 version of the firmware for Nucleo-H743 board

SCRIPT_NAME="compile_nucleo_h743.sh"
echo "Entering $SCRIPT_NAME"

bash ../common_make.sh config/boards/nucleo_h743/meta-info.env
