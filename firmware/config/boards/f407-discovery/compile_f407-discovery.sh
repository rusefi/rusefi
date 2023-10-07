#!/bin/bash

export EXTRA_PARAMS="-DDUMMY -DSHORT_BOARD_NAME=f407-discovery -DSTATIC_BOARD_ID=STATIC_BOARD_ID_F407_DISCOVERY"

bash ../common_make.sh f407-discovery ARCH_STM32F4
