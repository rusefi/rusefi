#!/bin/bash
# file gen_config_default.sh

bash gen_config_board.sh f407-discovery all rusefi.ini
exit $?

# todo gen_config.sh code duplication https://github.com/rusefi/rusefi/issues/2451
set -e




BOARDNAME=f407-discovery
SHORT_BOARDNAME=all
INI=rusefi.ini










echo "BOARDNAME=${BOARDNAME} SHORT_BOARDNAME=${SHORT_BOARDNAME}"
if [ "${SHORT_BOARDNAME}" = "all" ]; then
  SOMETHINGSOMETHING=false
else
  SOMETHINGSOMETHING=true
fi

bash gen_signature.sh all


java -DSystemOut.name=gen_config \
  -jar ../java_tools/ConfigDefinition.jar \
  -definition integration/rusefi_config.txt \
  -romraider integration \
  -tool gen_config.sh \
  -ts_destination tunerstudio \
	-board ${BOARDNAME} \
  -ts_output_name generated/${INI} \
  -cache ${SHORT_BOARDNAME} \
  -cache_zip_file tunerstudio/generated/cache.zip \
  -with_c_defines $SOMETHINGSOMETHING \
  -initialize_to_zero $SOMETHINGSOMETHING \
  -firing_order controllers/algo/firing_order.h \
  -signature tunerstudio/generated/signature_${SHORT_BOARDNAME}.txt \
  -signature_destination controllers/generated/signature_${SHORT_BOARDNAME}.h \
  -java_destination ../java_console/models/src/main/java/com/rusefi/config/generated/Fields.java \
  -enumInputFile controllers/algo/rusefi_enums.h \
	-enumInputFile controllers/algo/rusefi_hw_enums.h \
  -romraider_destination ../java_console/rusefi.xml \
  -c_defines        controllers/generated/rusefi_generated.h \
  -c_destination    controllers/generated/engine_configuration_generated_structures.h \
  -c_fsio_constants controllers/generated/fsio_enums_generated.def \
  -c_fsio_getters   controllers/generated/fsio_getters.def \
  -c_fsio_names     controllers/generated/fsio_names.def \
  -c_fsio_strings   controllers/generated/fsio_strings.def \
  -prepend config/boards/${BOARDNAME}/prepend.txt

[ $? -eq 0 ] || { echo "ERROR generating TunerStudio config for ${BOARDNAME}"; exit 1; }









exit 0
