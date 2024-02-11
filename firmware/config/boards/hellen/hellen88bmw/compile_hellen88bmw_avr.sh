#!/bin/bash

cd ..

export EXTRA_PARAMS="\
-DEFI_SOFTWARE_KNOCK=FALSE \
-DHAL_TRIGGER_USE_ADC=TRUE \
-DSTM32_ADC_USE_ADC3=TRUE \
"

bash ../common_make.sh config/boards/hellen/hellen88bmw/meta-info.env
