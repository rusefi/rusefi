#!/bin/bash

export DEBUG_LEVEL_OPT="-O0 -ggdb -g -Wl,--defsym=FLASH_SIZE=768k"
export EFI_LUA_LOOKUP="FALSE"

bash ../common_make.sh proteus ARCH_STM32F7
