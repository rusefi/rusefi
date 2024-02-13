#!/bin/bash

#
# in order to get fresh .ini and ramdisk_image one still has to manually invoke
# ./gen_config_board.sh config/boards/microrusefi mre_f4
#

export USE_OPENBLT=yes

bash ../common_make.sh microrusefi ARCH_STM32F4
