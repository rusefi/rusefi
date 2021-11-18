#!/bin/bash


java -DSystemOut.name=logs/gen_output_channels \
 -jar ../java_tools/ConfigDefinition.jar \
 -definition integration/output_channels.txt \
 -cache_zip_file tunerstudio/generated/cache.zip \
 -cache . \
 -c_destination console/binary/ts_outputs_generated.h