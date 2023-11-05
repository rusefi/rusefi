#!/bin/bash

export EXTRA_PARAMS="-DRAMDISK_INVALID -DEFI_LUA=FALSE"
export DEBUG_LEVEL_OPT="-O0 -ggdb -g"
export INCLUDE_ELF=yes
cd ..
bash ../common_make.sh hellen/alphax-8chan ARCH_STM32F4
