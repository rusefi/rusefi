#!/bin/bash

export PROJECT_BOARD=hellen/hellen-nb1
export PROJECT_CPU=ARCH_STM32F4
export EXTRA_PARAMS="-DSHORT_BOARD_NAME=hellen-nb1"

cd ..
bash ../common_make.sh
