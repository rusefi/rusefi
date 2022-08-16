#!/bin/bash
# This script files reads rusefi_config.txt and produces firmware persistent configuration headers
# the storage section of rusefi.ini is updated as well

BOARDNAME=cypress
SHORT_BOARDNAME=cypress
BOARD_DIR=config/boards/hellen/${BOARDNAME}







bash gen_signature.sh ${SHORT_BOARDNAME}

source gen_config_common.sh
echo "Using COMMON_GEN_CONFIG [$COMMON_GEN_CONFIG]"

java \
 -DSystemOut.name=logs/gen_config_hellen_cypress \
 $COMMON_GEN_CONFIG_PREFIX \
 -tool hellen_cypress_gen_config.bat \
 $COMMON_GEN_CONFIG \
 -ts_output_name generated/rusefi_${SHORT_BOARDNAME}.ini \
 -c_defines ${BOARD_DIR}/config/controllers/algo/rusefi_generated.h \
 -c_destination ${BOARD_DIR}/config/controllers/algo/engine_configuration_generated_structures.h \
 -signature tunerstudio/generated/signature_${SHORT_BOARDNAME}.txt \
 -signature_destination controllers/generated/signature_${SHORT_BOARDNAME}.h \
 -enumInputFile controllers/algo/rusefi_hw_enums.h \
 -board hellen_cypress \
 -prepend ${BOARD_DIR}/config/tunerstudio/generated/hellen_cypress_prefix.txt

[ $? -eq 0 ] || { echo "ERROR generating TunerStudio config for ${BOARDNAME}"; exit 1; }
