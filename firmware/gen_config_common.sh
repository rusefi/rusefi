
COMMON_GEN_CONFIG_PREFIX="-jar ../java_tools/ConfigDefinition.jar"

COMMON_GEN_CONFIG="
 -enumInputFile controllers/algo/engine_types.h \
 -enumInputFile controllers/algo/rusefi_enums.h \
 -readfile OUTPUTS_SECTION console/binary/generated/output_channels.ini \
 -readfile DATALOG_SECTION console/binary/generated/data_logs.ini \
 -ts_destination tunerstudio \
 -firing_order controllers/algo/firing_order.h \
 -triggerFolder ../unit_tests \
 -with_c_defines false \
 -initialize_to_zero false \
 -prepend console/binary/generated/total_live_data_generated.h \
 -prepend console/binary/generated/fragments_meta.h \
 -definition integration/rusefi_config.txt"
