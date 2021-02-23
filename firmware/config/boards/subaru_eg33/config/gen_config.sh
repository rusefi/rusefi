#/!bin/sh
# This batch files reads rusefi_config.txt and produses firmware persistent configuration headers
# the storage section of rusefi.ini is updated as well

BOARDNAME=subaru_eg33
SHORT_BOARDNAME=${BOARDNAME}_f7
BOARD_DIR=config/boards/${BOARDNAME}
BUILD_DIR=build_${BOARDNAME}

bash gen_signature.sh ${SHORT_BOARDNAME}

mkdir -p ${BUILD_DIR}

java \
 -DSystemOut.name=gen_config_subaru_eg33 \
 -Drusefi.generator.lazyfile.enabled=true \
 -jar ../java_tools/ConfigDefinition.jar \
 -definition integration/rusefi_config.txt \
 -cache ${SHORT_BOARDNAME} \
 -cache_zip_file tunerstudio/generated/cache.zip \
 -ts_destination tunerstudio \
 -tool ${BOARD_DIR}/config/gen_config.sh \
 -firing_order controllers/algo/firing_order.h \
 -with_c_defines false \
 -initialize_to_zero false \
 -ts_output_name generated/rusefi_${SHORT_BOARDNAME}.ini \
 -c_defines ${BOARD_DIR}/config/controllers/algo/rusefi_generated.h \
 -c_destination ${BOARD_DIR}/config/controllers/algo/engine_configuration_generated_structures.h \
 -signature tunerstudio/generated/signature_${SHORT_BOARDNAME}.txt \
 -signature_destination controllers/generated/signature_${SHORT_BOARDNAME}.h \
 -enumInputFile controllers/algo/rusefi_enums.h \
 -enumInputFile controllers/algo/rusefi_hw_enums.h \
 -board {BOARDNAME} \
 -prepend tunerstudio/generated/${BOARDNAME}_prefix.txt \
 -prepend config/boards/${BOARDNAME}/prepend.txt

[ $? -eq 0 ] || { echo "ERROR generating TunerStudio config for ${BOARDNAME}"; exit 1; }
