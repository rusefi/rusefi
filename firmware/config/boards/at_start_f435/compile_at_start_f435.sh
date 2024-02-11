#!/bin/bash

# STM32F4...

SCRIPT_NAME="compile_at_start_f435.sh"
echo "Entering $SCRIPT_NAME"

bash ../common_make.sh config/boards/at_start_f435/meta-info.env
