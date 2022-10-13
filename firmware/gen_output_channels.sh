#!/bin/bash

java -DSystemOut.name=logs/gen_output_channels \
 -jar ../java_tools/ConfigDefinition.jar \
 -prepend integration/rusefi_config.txt \
 -definition console/binary/output_channels.txt \
 -sd_destination console/binary_log/log_fields_generated.h \
 -output_lookup_file controllers/lua/generated/output_lookup_generated.cpp \
 -ts_outputs_section console/binary/
