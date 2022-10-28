#!/bin/bash

rm gen_live_documentation.log

java -DSystemOut.name=logs/gen_live_documentation \
 -cp ../java_tools/ConfigDefinition.jar \
 com.rusefi.ldmp.LiveDataProcessor integration/LiveData.yaml
[ $? -eq 0 ] || { echo "ERROR generating"; exit 1; }

bash gen_output_channels.sh
