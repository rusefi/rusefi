#!/bin/bash

cd ..

export EXTRA_PARAMS="\
-DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE \
"

bash ../common_make.sh config/boards/hellen/hellen88bmw/meta-info.env
