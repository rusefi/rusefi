#!/bin/bash

java -DSystemOut.name=logs/gen_output_channels \
 -jar ../java_tools/ConfigDefinition.jar \
 -prepend integration/rusefi_config.txt \
 -definition console/binary/output_channels.txt \
 -ts_outputs_section console/binary/
