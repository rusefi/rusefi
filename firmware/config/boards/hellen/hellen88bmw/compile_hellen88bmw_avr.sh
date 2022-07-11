#!/bin/bash

cd ..

export EXTRA_PARAMS="-DDUMMY \
 \
"


bash ../common_make.sh hellen/hellen88bmw ARCH_STM32F4
