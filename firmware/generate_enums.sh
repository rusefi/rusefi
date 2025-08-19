#!/bin/bash

rm -f generate_enums.log

rm -f controllers/generated/rusefi_enums_generated.h

java -DSystemOut.name=logs/generate_enums.log \
     -Ddefinition=integration/config_enums.json \
     -DoutputPathCpp=controllers/generated/rusefi_enums_generated.h \
     -DoutputPathIni=integration/config_generated_enums-$1.ini \
     -DoutputPathTxt=integration/config_generated_enums-$1.txt \
     -DoutputPathCfg=integration/config_generated_enums-$1.cfg \
     -cp ../java_tools/generate_enum/build/libs/generate_enum-all.jar \
     rusefi.GenerateEnum
