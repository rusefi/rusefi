#!/bin/bash

# STM32H743 version of the firmware for Nucleo-H743 board

SCRIPT_NAME="compile_nucleo_h743.sh"
echo "Entering $SCRIPT_NAME"

# Nucleo boards use MCO signal from St-Link and NOT oscillator - these need STM32_HSE_BYPASS

export EXTRA_PARAMS="-DDUMMY \
 -DEFI_INJECTOR_PIN3=Gpio::Unassigned \
 -DSTM32_HSE_BYPASS=TRUE \
 -DFIRMWARE_ID=\\\"nucleoH743\\\" \
 -DEFI_COMMUNICATION_PIN=Gpio::B7 \
 -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::B14"
export DEBUG_LEVEL_OPT="-O2"

bash ../common_make.sh nucleo_h743 ARCH_STM32H7
