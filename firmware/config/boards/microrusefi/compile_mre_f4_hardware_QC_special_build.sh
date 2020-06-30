#!/bin/sh

cd ../../..

export PROJECT_BOARD=microrusefi
export PROJECT_CPU=ARCH_STM32F4
export DEFAULT_ENGINE_TYPE=-DDEFAULT_ENGINE_TYPE=MRE_BOARD_NEW_TEST
export EXTRA_PARAMS=-DHW_CHECK_MODE=TRUE

sh config/boards/common_make.sh


