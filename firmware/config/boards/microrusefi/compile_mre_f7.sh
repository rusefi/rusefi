#!/bin/bash

export PROJECT_BOARD=microrusefi
export PROJECT_CPU=ARCH_STM32F7
export EXTRA_PARAMS="-DSHORT_BOARD_NAME=mre_f7"

bash ../common_make.sh
