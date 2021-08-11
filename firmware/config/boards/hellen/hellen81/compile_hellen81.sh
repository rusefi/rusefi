#!/bin/bash

export PROJECT_BOARD=hellen/hellen81
export PROJECT_CPU=ARCH_STM32H7
export EXTRA_PARAMS="-DSHORT_BOARD_NAME=hellen81"

cd ..
bash ../common_make.sh
