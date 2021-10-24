#!/bin/bash

echo "This batch files reads rusefi_enums.h and produces auto_generated_enums.* files"

rm gen_enum_to_string.log

java -DSystemOut.name=logs/gen_java_enum -cp ../java_tools/enum2string.jar com.rusefi.ToJavaEnum -enumInputFile controllers/algo/live_data_ids.h -outputPath ../java_console/io/src/main/java/com/rusefi/enums
[ $? -eq 0 ] || { echo "ERROR generating live data ids"; exit 1; }

java -DSystemOut.name=logs/gen_java_enum -cp ../java_tools/enum2string.jar com.rusefi.ToJavaEnum -enumInputFile controllers/sensors/sensor_type.h -outputPath ../java_console/io/src/main/java/com/rusefi/enums
[ $? -eq 0 ] || { echo "ERROR generating sensors"; exit 1; }


java -DSystemOut.name=logs/gen_enum_to_string \
	-jar ../java_tools/enum2string.jar \
	-outputPath controllers/algo \
	-generatedFile codes \
	-enumInputFile controllers/algo/obd_error_codes.h

[ $? -eq 0 ] || { echo "ERROR generating obd_error_codes"; exit 1; }

java -DSystemOut.name=logs/gen_enum_to_string \
	-jar ../java_tools/enum2string.jar \
	-outputPath controllers/algo \
	-generatedFile commonenum \
	-enumInputFile controllers/algo/rusefi_enums.h

[ $? -eq 0 ] || { echo "ERROR generating enums"; exit 1; }

java -DSystemOut.name=logs/gen_enum_to_string \
	-jar ../java_tools/enum2string.jar \
	-outputPath controllers/algo \
	-generatedFile enginetypes \
	-enumInputFile controllers/algo/engine_types.h

[ $? -eq 0 ] || { echo "ERROR generating enums"; exit 1; }

# TODO: rearrange enums so that we have WAY less duplicated generated code? at the moment too many enums are generated 4 times

java -DSystemOut.name=logs/gen_enum_to_string \
	-jar ../java_tools/enum2string.jar \
	-outputPath controllers/algo \
	-enumInputFile controllers/algo/rusefi_hw_enums.h \

[ $? -eq 0 ] || { echo "ERROR generating hw_enums"; exit 1; }

java -DSystemOut.name=logs/gen_enum_to_string \
	-jar ../java_tools/enum2string.jar \
	-outputPath controllers/sensors \
	-generatedFile sensor \
	-enumInputFile controllers/sensors/sensor_type.h

[ $? -eq 0 ] || { echo "ERROR generating sensors"; exit 1; }

pwd
cd config/boards/kinetis/config
bash kinetis_gen_enum_to_string.sh
cd ../../../..

cd config/boards/hellen/cypress/config
bash hellen_cypress_gen_enum_to_string.sh
cd ../../../../..

bash config/boards/subaru_eg33/config/gen_enum_to_string.sh
