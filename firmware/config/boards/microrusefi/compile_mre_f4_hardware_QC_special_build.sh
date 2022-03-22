#!/bin/bash

export PROJECT_BOARD=microrusefi
export PROJECT_CPU=ARCH_STM32F4
export EXTRA_PARAMS="-DHW_CHECK_MODE=TRUE -DANALOG_HW_CHECK_MODE=TRUE -DHW_CHECK_ALAWAYS_STIMULATE=TRUE -DSHORT_BOARD_NAME=mre_f4 -DRAMDISK_INVALID"

export DEFAULT_ENGINE_TYPE=-DDEFAULT_ENGINE_TYPE=MRE_BOARD_NEW_TEST

# this QC configuration is used to assert our status with debug info
export DEBUG_LEVEL_OPT="-O0 -ggdb -g"

bash ../common_make.sh


