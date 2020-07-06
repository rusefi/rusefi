#!/bin/sh

cd ../../..

export PROJECT_BOARD=microrusefi
export PROJECT_CPU=ARCH_STM32F4
export DEFAULT_ENGINE_TYPE = -DDEFAULT_ENGINE_TYPE=BMW_M73_MRE_SLAVE
export EXTRA_PARAMS=-DDUMMY -DEFI_CANBUS_SLAVE=TRUE -DSHORT_BOARD_NAME=mre

sh config/boards/common_make.sh


