#!/usr/bin/env bash

#
# in order to get fresh .ini and ramdisk_image one still has to manually invoke
# ./gen_config_board.sh config/boards/microrusefi mre_f4
#

export USE_OPENBLT=yes
export SHORT_BOARD_NAME="mre_f4"
export PROJECT_BOARD="microrusefi"
export PROJECT_CPU="ARCH_STM32F4"
