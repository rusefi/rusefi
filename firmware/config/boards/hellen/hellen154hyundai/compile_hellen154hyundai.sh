#!/bin/bash

export PROJECT_BOARD=hellen/hellen154hyundai
export PROJECT_CPU=ARCH_STM32F4
export EXTRA_PARAMS="-DSHORT_BOARD_NAME=hellen154hyundai"

cd ..
bash ../common_make.sh
