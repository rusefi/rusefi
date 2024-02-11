#!/bin/bash

# STM32F4...

SCRIPT_NAME="compile_nucleo_f429.sh"
echo "Entering $SCRIPT_NAME"

bash ../common_make.sh config/boards/nucleo_f429/meta-info.env
