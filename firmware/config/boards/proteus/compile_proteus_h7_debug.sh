#!/bin/bash

export DEBUG_LEVEL_OPT="-O0 -ggdb -g"
export INCLUDE_ELF=yes
bash ../common_make.sh proteus ARCH_STM32H7
