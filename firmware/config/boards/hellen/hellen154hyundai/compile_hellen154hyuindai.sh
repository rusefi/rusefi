#!/bin/bash

export PROJECT_BOARD=hellen/hellen121nissan
export PROJECT_CPU=ARCH_STM32F4
export EXTRA_PARAMS="-DSHORT_BOARD_NAME=hellen121nissan"

cd ..
bash ../common_make.sh
