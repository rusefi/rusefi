#!/bin/bash

rm gen_live_documentation.log

if [ -n "$1" ]; then
  echo "With prepend $1"
  EXTRA_PREPEND="-DUsagesReader.extra_prepend=$1"
else

fi

java -DSystemOut.name=logs/gen_live_documentation \
 ${EXTRA_PREPEND} \
 -cp ../java_tools/ConfigDefinition.jar \
 com.rusefi.ldmp.UsagesReader integration/LiveData.yaml
[ $? -eq 0 ] || { echo "ERROR generating"; exit 1; }

bash gen_output_channels.sh
