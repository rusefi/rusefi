#!/bin/bash

export PROJECT_BOARD=hellen/hellen64_miataNA6_94
export PROJECT_CPU=ARCH_STM32F4
export EXTRA_PARAMS="-DSHORT_BOARD_NAME=hellenNA6"

cd ..
bash ../common_make.sh
