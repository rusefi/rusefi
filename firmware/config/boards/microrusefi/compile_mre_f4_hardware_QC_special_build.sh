#!/bin/bash

export PROJECT_BOARD=microrusefi
export PROJECT_CPU=ARCH_STM32F4
export EXTRA_PARAMS="-DHW_CHECK_MODE=TRUE -DSHORT_BOARD_NAME=mre_f4"

export DEFAULT_ENGINE_TYPE=-DDEFAULT_ENGINE_TYPE=MRE_BOARD_NEW_TEST

# this QC configuration is used to assert our status with debug info
export DEBUG_LEVEL_OPT="-O0 -ggdb -g3"

bash ../common_make.sh


