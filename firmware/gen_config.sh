#!/bin/sh

#set -x
#TS_PATH="/home/<user>/TunerStudioProjects/"

echo "This batch files reads rusefi_config.txt and produses firmware persistent configuration headers"
echo "the storage section of rusefi.ini is updated as well"

rm gen_config.log
rm gen_config_board.log

mkdir build

java -DSystemOut.name=gen_config \
 -Drusefi.generator.lazyfile.enabled=true \
 -jar ../java_tools/ConfigDefinition.jar \
 -definition integration/rusefi_config.txt \
 -romraider integration \
 -ts_destination tunerstudio \
 -cache tunerstudio/cache \
 -with_c_defines false \
 -initialize_to_zero false \
 -tool gen_config.sh \
 -c_defines        controllers/generated/rusefi_generated.h \
 -firing_order     controllers/algo/firing_order.h \
 -c_destination    controllers/generated/engine_configuration_generated_structures.h \
 -c_fsio_constants controllers/generated/fsio_enums_generated.def \
 -c_fsio_getters   controllers/generated/fsio_getters.def \
 -c_fsio_names     controllers/generated/fsio_names.def \
 -c_fsio_strings   controllers/generated/fsio_strings.def \
 -java_destination ../java_console/models/src/com/rusefi/config/generated/Fields.java \
 -romraider_destination ../java_console/rusefi.xml

[ $? -eq 0 ] || (echo "ERROR generating"; exit $?)

if [ -z "${TS_PATH}" ]; then
 echo "TS_PATH not defined"
else
 echo "This would automatically copy latest file to 'dev' TS project at ${TS_PATH}"
 cp -v tunerstudio/rusefi.ini $TS_PATH/dev/projectCfg/mainController.ini
 cp -v tunerstudio/rusefi_microrusefi.ini $TS_PATH/dev_mre/projectCfg/mainController.ini
fi

for BOARD in "microrusefi" "frankenso" "prometheus" "proteus"; do
 sh gen_config_board.sh $BOARD
 [ $? -eq 0 ] || (echo "ERROR generating $BOARD"; exit $?)
done

cd config/boards/kinetis/config
sh gen_config.sh

exit 0
