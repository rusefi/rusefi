#!/bin/sh

#set -x
#TS_PATH="/home/<user>/TunerStudioProjects/"

echo "This batch files reads rusefi_config.txt and produses firmware persistent configuration headers"
echo "the storage section of rusefi.ini is updated as well"

rm gen_config.log
rm gen_config_board.log

echo "lazy is broken - TS input is not considered a change"
rm build/config.gen

mkdir build

java -DSystemOut.name=gen_config \
	-Drusefi.generator.lazyfile.enabled=true \
	-jar ../java_tools/ConfigDefinition.jar \
	-definition integration/rusefi_config.txt \
	-romraider integration \
	-ts_destination tunerstudio \
	-with_c_defines false \
	-initialize_to_zero false \
	-c_defines        controllers/generated/rusefi_generated.h \
	-c_destination    controllers/generated/engine_configuration_generated_structures.h \
	-c_fsio_constants controllers/generated/fsio_enums_generated.def \
	-c_fsio_getters   controllers/generated/fsio_getters.def \
	-c_fsio_names     controllers/generated/fsio_names.def \
	-c_fsio_strings   controllers/generated/fsio_strings.def \
	-java_destination ../java_console/models/src/com/rusefi/config/generated/Fields.java \
	-romraider_destination ../java_console/rusefi.xml \
	-skip build/config.gen

[ $? -eq 0 ] || (echo "ERROR generating"; exit $?)

if [ -z "${TS_PATH}" ]; then
	echo "TS_PATH not defined"
else
	echo "This would automatically copy latest file to 'dev' TS project at ${TS_PATH}"
	cp -v tunerstudio/rusefi.ini $TS_PATH/dev/projectCfg/mainController.ini
	cp -v tunerstudio/rusefi_microrusefi.ini $TS_PATH/dev_mre/projectCfg/mainController.ini
fi

./gen_config_board.sh microrusefi
[ $? -eq 0 ] || (echo "ERROR generating microrusefi"; exit $?)

./gen_config_board.sh frankenso
[ $? -eq 0 ] || (echo "ERROR generating frankenso"; exit $?)

./gen_config_board.sh prometheus
[ $? -eq 0 ] || (echo "ERROR generating prometheus"; exit $?)

#cd config\boards\kinetis\config
#!gen_config.bat

exit 0
