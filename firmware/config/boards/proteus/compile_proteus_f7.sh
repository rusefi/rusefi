#!/bin/sh

cd ../../..

export LDSCRIPT=config/boards/NUCLEO_F767/STM32F76xxI.ld

export PROJECT_BOARD=proteus
export PROJECT_CPU=ARCH_STM32F7
export EXTRA_PARAMS=-DSHORT_BOARD_NAME=pro

sh config/boards/common_make.sh
