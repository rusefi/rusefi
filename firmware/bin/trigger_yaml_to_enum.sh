#!/usr/bin/env bash

OUTPUT=$1
shift
INPUT_FILES=$@

FILES=()

if [ -z "$INPUT_FILES" ]; then
  FILES+=(-input ../firmware/config/triggers.yaml)
else
  for f in ${INPUT_FILES[@]}; do \
    INPUTS+=(-input "$f")
  done
fi

java -cp ../java_tools/enum_to_string/build/libs/enum_to_string-all.jar com.rusefi.YamlToEnum \
-yamlKey name \
-enumName trigger_type_e \
${FILES[@]} \
-prepend TT_TOOTHED_WHEEL -append TT_UNUSED \
-output $OUTPUT
