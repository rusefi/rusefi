#!/bin/bash

export PROJECT_BOARD=hellen/hellen121vag
export PROJECT_CPU=ARCH_STM32F4
export EXTRA_PARAMS="-DSHORT_BOARD_NAME=hellen121vag"

cd ..
bash ../common_make.sh
