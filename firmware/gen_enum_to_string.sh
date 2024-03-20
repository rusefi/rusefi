#!/bin/bash

# This script reads various C/C++ enums and produces auto_generated_enums.* files"

set -euo pipefail

rm -f gen_enum_to_string.log

java -DSystemOut.name=logs/gen_java_enum -cp ../java_tools/enum2string.jar com.rusefi.ToJavaEnum -enumInputFile controllers/sensors/sensor_type.h -outputPath ../java_console/io/src/main/java/com/rusefi/enums

java -DSystemOut.name=logs/gen_java_enum -cp ../java_tools/enum2string.jar com.rusefi.ToJavaEnum -enumInputFile controllers/algo/engine_types.h   -outputPath ../java_console/io/src/main/java/com/rusefi/enums -definition integration/rusefi_config.txt

java -DSystemOut.name=logs/gen_enum_to_string \
	-jar ../java_tools/enum2string.jar \
	-outputPath controllers/algo \
	-generatedFile commonenum \
	-enumInputFile controllers/algo/rusefi_enums.h

java -DSystemOut.name=logs/gen_enum_to_string \
	-jar ../java_tools/enum2string.jar \
	-outputPath controllers/trigger/decoders \
	-generatedFile sync_edge \
	-enumInputFile controllers/trigger/decoders/sync_edge.h

java -DSystemOut.name=logs/gen_enum_to_string \
	-jar ../java_tools/enum2string.jar \
	-outputPath controllers/algo \
	-generatedFile enginetypes \
	-enumInputFile controllers/algo/engine_types.h

# TODO: rearrange enums so that we have WAY less duplicated generated code? at the moment too many enums are generated 4 times

java -DSystemOut.name=logs/gen_enum_to_string \
	-jar ../java_tools/enum2string.jar \
	-outputPath controllers/algo \
	-enumInputFile controllers/algo/rusefi_hw_enums.h \

java -DSystemOut.name=logs/gen_enum_to_string \
	-jar ../java_tools/enum2string.jar \
	-outputPath controllers/sensors \
	-generatedFile sensor \
	-enumInputFile controllers/sensors/sensor_type.h
