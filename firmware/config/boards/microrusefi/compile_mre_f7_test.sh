#!/bin/sh

cd ../../..

export LDSCRIPT="config/boards/NUCLEO_F767/STM32F76xxI.ld"

export PROJECT_BOARD=microrusefi
export PROJECT_CPU=ARCH_STM32F7
export DEFAULT_ENGINE_TYPE="-DDEFAULT_ENGINE_TYPE=MRE_BOARD_TEST"
export EXTRA_PARAMS=-DSHORT_BOARD_NAME=mre

sh config/boards/common_make.sh
