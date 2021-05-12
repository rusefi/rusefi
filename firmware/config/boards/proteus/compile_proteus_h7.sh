#!/bin/bash

export PROJECT_BOARD=proteus
export PROJECT_CPU=ARCH_STM32H7
export EXTRA_PARAMS=-DSHORT_BOARD_NAME=proteus_h7

bash ../common_make.sh
