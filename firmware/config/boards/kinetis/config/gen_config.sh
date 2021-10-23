#!/bin/bash
# This script files reads rusefi_config.txt and produces firmware persistent configuration headers
# the storage section of rusefi.ini is updated as well
#
# many of the files generated here require time consuming full compilation thus we have an aggressive caching mechanism
# to make sure that we do not regenerate for no reason
# the primary input files are rusefi_config.txt and rusefi.input, also mapping.yaml etc
# see inside cache.zip for all input files
#

cd ../../../..

pwd

bash gen_signature.sh kin

source gen_config_common.sh
echo "Using COMMON_GEN_CONFIG [$COMMON_GEN_CONFIG]"

java \
 -DSystemOut.name=gen_config_kinetis \
 $COMMON_GEN_CONFIG \
 -cache kinetis \
 -tool kinetis_gen_config.bat \
 -with_c_defines false \
 -initialize_to_zero false \
 -ts_output_name generated/rusefi_kinetis.ini \
 -c_defines config/boards/kinetis/config/controllers/algo/rusefi_generated.h \
 -c_destination config/boards/kinetis/config/controllers/algo/engine_configuration_generated_structures.h \
 -signature tunerstudio/generated/signature_kin.txt \
 -signature_destination controllers/generated/signature_kin.h \
 -enumInputFile controllers/algo/rusefi_hw_enums.h \
 -board kinetis \
 -prepend config/boards/kinetis/config/rusefi_config_kinetis.txt

[ $? -eq 0 ] || { echo "ERROR generating TunerStudio config for kinetis"; exit 1; }
