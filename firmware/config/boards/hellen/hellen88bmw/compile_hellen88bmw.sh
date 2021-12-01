#!/bin/bash

export PROJECT_BOARD=hellen/hellen88bmw
export PROJECT_CPU=ARCH_STM32F4
export EXTRA_PARAMS="-DSHORT_BOARD_NAME=hellen88bmw"

cd ..
bash ../common_make.sh
