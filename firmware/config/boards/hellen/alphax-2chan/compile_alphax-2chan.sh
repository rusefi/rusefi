#!/bin/bash

export PROJECT_BOARD=hellen/alphax-2chan
export PROJECT_CPU=ARCH_STM32F4
export EXTRA_PARAMS="-DSHORT_BOARD_NAME=AlphaX-2chan"

cd ..
bash ../common_make.sh
