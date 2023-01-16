#!/bin/bash

rm gen_live_documentation.log

java -DSystemOut.name=logs/gen_live_documentation \
 -cp ../java_tools/ConfigDefinition.jar \
 com.rusefi.ldmp.LiveDataProcessor integration/LiveData.yaml
[ $? -eq 0 ] || { echo "ERROR generating"; exit 1; }

java -DSystemOut.name=logs/gen_java_enum -cp ../java_tools/enum2string.jar com.rusefi.ToJavaEnum -enumInputFile console/binary/generated/live_data_ids.h -outputPath ../java_console/io/src/main/java/com/rusefi/enums
[ $? -eq 0 ] || { echo "ERROR generating live data ids"; exit 1; }
