#!/bin/sh

cd ../../..

export PROJECT_BOARD=microrusefi
export PROJECT_CPU=ARCH_STM32F4
export EXTRA_PARAMS=-DSHORT_BOARD_NAME=mre



sh config/boards/common_make.sh
