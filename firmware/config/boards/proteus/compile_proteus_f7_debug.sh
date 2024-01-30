#!/usr/bin/env bash

export EXTRA_PARAMS="-DEFI_LUA=FALSE"
export DEBUG_LEVEL_OPT="-O0 -ggdb -g -Wl,--defsym=FLASH_SIZE=768k"
export INCLUDE_ELF=yes
export PROJECT_BOARD="proteus"
export PROJECT_CPU="ARCH_STM32F7"
export SHORT_BOARD_NAME=proteus_f7
