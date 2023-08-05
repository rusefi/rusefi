#!/bin/bash

export EXTRA_PARAMS="-DEFI_LUA=FALSE -DSHORT_BOARD_NAME=f407-discovery"
export DEBUG_LEVEL_OPT="-O0 -ggdb -g"

bash ../common_make.sh f407-discovery ARCH_STM32F4
