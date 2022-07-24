#!/bin/bash

# does not fit into 512K flash, and we use 512K because of st DFU client bug :(
# export USE_OPENBLT=yes

bash ../common_make.sh proteus ARCH_STM32F7
