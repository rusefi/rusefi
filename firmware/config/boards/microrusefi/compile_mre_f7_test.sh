#!/bin/bash

export VAR_DEF_ENGINE_TYPE="-DDEFAULT_ENGINE_TYPE=MRE_BOARD_TEST"

bash ../common_make.sh microrusefi ARCH_STM32F7
