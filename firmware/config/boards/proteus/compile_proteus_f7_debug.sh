#!/bin/bash

export EXTRA_PARAMS="-DEFI_LUA_LOOKUP=FALSE"

bash ../common_make.sh proteus ARCH_STM32F7
