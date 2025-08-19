#!/bin/bash

if which grealpath >/dev/null 2>&1; then alias realpath='grealpath'; fi
FDIR=$(realpath $(dirname "$0")/..)
cd "$FDIR" || exit
echo -e "$FDIR\n"
echo -e "\n"

rm -f generate_enums.log

rm -f controllers/generated/rusefi_enums_generated.h

java -DSystemOut.name=logs/generate_enums.log \
     -Ddefinition=firmware/integration/config_enums.json \
     -DoutputPathCpp=firmware/controllers/generated/rusefi_enums_generated.h \
     -DoutputPathIni=firmware/integration/config_generated_enums-$1.ini \
     -DoutputPathTxt=firmware/integration/config_generated_enums-$1.txt \
     -DoutputPathCfg=firmware/integration/config_generated_enums-$1.cfg \
     -cp java_tools/generate_enum/build/libs/generate_enum-all.jar \
     rusefi.GenerateEnum
