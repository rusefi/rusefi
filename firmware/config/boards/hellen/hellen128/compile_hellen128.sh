#!/bin/bash

export PROJECT_BOARD=hellen/hellen128
export PROJECT_CPU=ARCH_STM32F4
export EXTRA_PARAMS="-DSHORT_BOARD_NAME=hellen128"

cd ..
bash ../common_make.sh
