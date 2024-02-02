#!/bin/bash

export USE_OPENBLT=yes

cd ..
bash ../common_make.sh alphax-8chan ARCH_STM32F7 config/boards/hellen/alphax-8chan
