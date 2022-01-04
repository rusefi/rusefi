#!/bin/bash


source gen_config_common.sh
echo "Using COMMON_GEN_CONFIG [$COMMON_GEN_CONFIG]"

java -DSystemOut.name=logs/gen_output_channels \
 -jar ../java_tools/ConfigDefinition.jar \
 -prepend integration/rusefi_config.txt \
 -definition console/binary/output_channels.txt \
 -output_lookup_file controllers/lua/generated/output_lookup_generated.cpp \
 -ts_outputs_section console/binary/ \
 -with_c_defines false \
 -c_destination console/binary/ts_outputs_generated.h