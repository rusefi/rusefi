#!/bin/bash

export USE_OPENBLT=yes
export DEBUG_LEVEL_OPT="-Os -ggdb -g"

bash ../common_make.sh config/boards/proteus/meta-info-proteus_f7.env
