#!/bin/bash

export PROJECT_BOARD=hellen/hellen81hd
export PROJECT_CPU=ARCH_STM32F4
export EXTRA_PARAMS="-DSHORT_BOARD_NAME=hellen81hd"

cd ..
bash ../common_make.sh
