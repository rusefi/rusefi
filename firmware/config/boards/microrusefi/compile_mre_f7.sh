#!/bin/bash

cd ../../..

export PROJECT_BOARD=microrusefi
export PROJECT_CPU=ARCH_STM32F7
export EXTRA_PARAMS="-DSHORT_BOARD_NAME=mre_f7 -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE"

export LDSCRIPT="config/boards/NUCLEO_F767/STM32F76xxI.ld"

bash config/boards/common_make.sh
