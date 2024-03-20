#!/bin/bash
# This script files reads rusefi_config.txt and produces firmware persistent configuration headers
# the storage section of rusefi.ini is updated as well

BOARDNAME=cypress
SHORT_BOARD_NAME=cypress
BOARD_DIR=config/boards/${BOARDNAME}















  INI="rusefi_${SHORT_BOARD_NAME}.ini"




bash gen_signature.sh ${SHORT_BOARD_NAME}

source gen_config_common.sh
echo "Using COMMON_GEN_CONFIG [$COMMON_GEN_CONFIG]"

java \
 $COMMON_GEN_CONFIG_PREFIX \
 -tool hellen_cypress_gen_config.bat \
 $COMMON_GEN_CONFIG \
 -c_defines controllers/generated/rusefi_generated_cypress.h \
 -c_destination controllers/generated/engine_configuration_generated_structures_cypress.h \
 -enumInputFile controllers/algo/rusefi_hw_enums.h

[ $? -eq 0 ] || { echo "ERROR generating TunerStudio config for ${BOARDNAME}"; exit 1; }
