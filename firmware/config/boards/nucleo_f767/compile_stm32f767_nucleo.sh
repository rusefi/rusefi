#!/bin/bash

# STM32F767 version of the firmware for Nucleo-F767ZI board

SCRIPT_NAME="compile_nucleo_f767.sh"
echo "Entering $SCRIPT_NAME"

# Nucleo boards use MCO signal from St-Link and NOT oscillator - these need STM32_HSE_BYPASS

bash ../common_script.sh config/boards/nucleo_f767/meta-info-stm32f767_nucleo.env
