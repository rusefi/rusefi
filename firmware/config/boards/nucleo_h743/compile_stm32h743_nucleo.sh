#!/bin/bash

# STM32H743 version of the firmware for Nucleo-H743 board

SCRIPT_NAME="compile_nucleo_h743.sh"
echo "Entering $SCRIPT_NAME"

export PROJECT_BOARD=nucleo_h743
export PROJECT_CPU=ARCH_STM32H7
export EXTRA_PARAMS="-DDUMMY \
 -DEFI_INJECTOR_PIN3=GPIO_UNASSIGNED \
 -DFIRMWARE_ID=\\\"nucleoH743\\\" \
 -DEFI_COMMUNICATION_PIN=GPIOB_7 \
 -DSTATUS_LOGGING_BUFFER_SIZE=1400 \
 -DLED_CRITICAL_ERROR_BRAIN_PIN=GPIOB_14"
export DEBUG_LEVEL_OPT="-O2"

bash ../common_make.sh
