#!/bin/bash

cd ../../..

export PROJECT_BOARD=microrusefi
export PROJECT_CPU=ARCH_STM32F4
export EXTRA_PARAMS="-DSHORT_BOARD_NAME=mre_f4 -DEFI_CONSOLE_TX_BRAIN_PIN=GPIOB_10 -DEFI_CONSOLE_RX_BRAIN_PIN=GPIOB_11"



bash config/boards/common_make.sh
