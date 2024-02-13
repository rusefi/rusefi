#!/bin/bash

export USE_OPENBLT=yes
export DEBUG_LEVEL_OPT="-Os -ggdb -g"

bash ../common_make.sh proteus ARCH_STM32F7
