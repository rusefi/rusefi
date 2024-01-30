#!/usr/bin/env bash

export EXTRA_PARAMS="-DRAMDISK_INVALID -DEFI_LUA=FALSE"
export DEBUG_LEVEL_OPT="-O0 -ggdb -g"

# export USE_OPENBLT=yes
export INCLUDE_ELF=yes
export PROJECT_BOARD="microrusefi"
export PROJECT_CPU="ARCH_STM32F4"
export SHORT_BOARD_NAME=mre_f4
