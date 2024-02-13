#!/bin/bash

export USE_OPENBLT=yes

cd ..
bash ../common_make.sh alphax-2chan ARCH_STM32F4 config/boards/hellen/alphax-2chan
