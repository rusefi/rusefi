#!/bin/bash

export EXTRA_PARAMS="-DEFI_LUA=FALSE"
export DEBUG_LEVEL_OPT="-O0 -ggdb -g"

bash ../common_make.sh proteus ARCH_STM32F4
