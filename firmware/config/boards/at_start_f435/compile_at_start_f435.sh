#!/bin/bash

# STM32F4...

SCRIPT_NAME="compile_at_start_f435.sh"
echo "Entering $SCRIPT_NAME"

bash ../common_make.sh at_start_f435 ARCH_AT32F4
