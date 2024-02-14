#!/bin/bash

export DEBUG_LEVEL_OPT="-O0 -ggdb -g"
export INCLUDE_ELF=yes
bash ../common_make.sh config/boards/proteus/meta-info-proteus_h7_debug.env
