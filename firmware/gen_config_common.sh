
COMMON_GEN_CONFIG_PREFIX="-jar ../java_tools/ConfigDefinition.jar"

COMMON_GEN_CONFIG="
 -enumInputFile controllers/algo/engine_types.h \
 -enumInputFile controllers/algo/rusefi_enums.h \
 -readfile OUTPUTS_SECTION_FROM_FILE console/binary/generated/output_channels.ini \
 -readfile DATALOG_SECTION_FROM_FILE console/binary/generated/data_logs.ini \
 -readfile LIVE_DATA_MENU_FROM_FILE console/binary/generated/fancy_menu.ini \
 -readfile LIVE_DATA_PANELS_FROM_FILE console/binary/generated/fancy_content.ini \
 -ts_destination tunerstudio \
 -firing_order controllers/algo/firing_order.h \
 -triggerFolder ../unit_tests \
 -with_c_defines false \
 -initialize_to_zero false \
 -prepend console/binary/generated/total_live_data_generated.h \
 -signature tunerstudio/generated/signature_${SHORT_BOARDNAME}.txt \
 -signature_destination controllers/generated/signature_${SHORT_BOARDNAME}.h \
 -ts_output_name generated/${INI} \
 -prepend ${BOARD_DIR}/prepend.txt \
 -board ${BOARDNAME} \
 -definition integration/rusefi_config.txt"
