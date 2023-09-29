#!/bin/bash

export EXTRA_PARAMS="-DRAMDISK_INVALID -DEFI_LUA=FALSE"
export DEBUG_LEVEL_OPT="-O0 -ggdb -g"

cd ..
bash ../common_make.sh hellen/hellen-112-17 ARCH_STM32F4
