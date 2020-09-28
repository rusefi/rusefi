#!/bin/bash

export PROJECT_BOARD=proteus
export PROJECT_CPU=ARCH_STM32F7
export EXTRA_PARAMS=-DSHORT_BOARD_NAME=proteus_f7

export LDSCRIPT=config/boards/NUCLEO_F767/STM32F76xxI.ld

bash config/boards/common_make.sh
