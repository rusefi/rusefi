#!/bin/sh

cd ../../..

export PROJECT_BOARD=microrusefi
export PROJECT_CPU=ARCH_STM32F7
export EXTRA_PARAMS=-DSHORT_BOARD_NAME=mre
export DEFAULT_ENGINE_TYPE="-DDEFAULT_ENGINE_TYPE=MRE_BOARD_TEST"
export LDSCRIPT="config/boards/NUCLEO_F767/STM32F76xxI.ld"

sh config/boards/common_make.sh
