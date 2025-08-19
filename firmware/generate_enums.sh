#!/bin/bash

rm -f generate_enums.log

rm -f controllers/generated/rusefi_enums_generated.h

java -DSystemOut.name=logs/generate_enums.log \
     -Ddefinition=integration/rusefi_config.txt -DoutputPath=controllers/generated/rusefi_enums_generated.h \
     -cp ../java_tools/generate_enum/build/libs/generate_enum-all.jar \
     rusefi.GenerateEnum
