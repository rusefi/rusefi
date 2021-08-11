#!/bin/bash

echo "This batch files reads rusefi_enums.h and produces auto_generated_enums.* files"

rm gen_enum_to_string.log

java -DSystemOut.name=gen_enum_to_string \
	-jar ../java_tools/enum2string.jar \
	-outputPath controllers/algo \
	-generatedFile codes \
	-enumInputFile controllers/algo/obd_error_codes.h

java -DSystemOut.name=gen_enum_to_string \
	-jar ../java_tools/enum2string.jar \
	-outputPath controllers/algo \
	-generatedFile commonenum \
	-enumInputFile controllers/algo/rusefi_enums.h

# TODO: rearrange enums so that we have WAY less duplicated generated code? at the moment too many enums are generated 4 times

java -DSystemOut.name=gen_enum_to_string \
	-jar ../java_tools/enum2string.jar \
	-outputPath controllers/algo \
	-enumInputFile controllers/algo/rusefi_hw_enums.h \

pwd
cd config/boards/kinetis/config
bash kinetis_gen_enum_to_string.sh
cd ../../../..

cd config/boards/hellen/cypress/config
bash hellen_cypress_gen_enum_to_string.sh
cd ../../../../..

bash config/boards/subaru_eg33/config/gen_enum_to_string.sh
