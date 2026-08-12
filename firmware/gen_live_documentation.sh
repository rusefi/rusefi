#!/bin/bash

cd $(dirname "$0")

rm gen_live_documentation.log

java -DSystemOut.name=logs/gen_live_documentation \
 -cp ../java_tools/configuration_definition_base/build/libs/config_definition_base-all.jar \
 com.rusefi.SimpleDefinesProcessor integration/ts_protocol.txt

java -DSystemOut.name=logs/gen_live_documentation \
 -cp ../java_tools/configuration_definition_base/build/libs/config_definition_base-all.jar \
 -DLiveDataProcessor.extra_prepend=${BOARD_DIR}/prepend.txt \
 com.rusefi.ldmp.LiveDataProcessor \
  integration/LiveData.yaml${EXTRA_LIVE_DATA_FILE} \
  integration/rusefi_config_trigger.txt \
  ../unit_tests/test-framework/trigger_meta_generated.h \
 ../java_tools/trigger-image/src/main/java/com/rusefi/config/generated/ \
 "./${META_OUTPUT_ROOT_FOLDER}"
[ $? -eq 0 ] || { echo "ERROR generating with LiveDataProcessor"; exit 1; }

java -DSystemOut.name=logs/gen_java_enum -cp ../java_tools/enum_to_string/build/libs/enum_to_string-all.jar \
 com.rusefi.ToJavaEnum -enumInputFile \
 ${META_OUTPUT_ROOT_FOLDER}console/binary/generated/live_data_ids.h \
 -outputPath \
 ../java_console/io/src/main/java/com/rusefi/enums
[ $? -eq 0 ] || { echo "ERROR generating live data ids"; exit 1; }

# live_data_ids.h is generated above so order of operations is important here!

./gen_enum_to_string.sh
./gen_perf_trace_enum.sh
