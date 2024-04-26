#!/bin/bash

# STM32F4...

SCRIPT_NAME="compile_nucleo_f429.sh"
echo "Entering $SCRIPT_NAME"

export USE_OPENBLT=yes

bash ../common_make.sh nucleo_f429 ARCH_STM32F4
