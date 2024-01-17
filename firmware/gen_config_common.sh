
COMMON_GEN_CONFIG_PREFIX=" -DSystemOut.name=logs/gen_config_${SHORT_BOARDNAME} \
 -jar ../java_tools/ConfigDefinition.jar"

COMMON_GEN_CONFIG="
 -enumInputFile controllers/algo/engine_types.h \
 -enumInputFile controllers/algo/rusefi_enums.h \
 -readfile OUTPUTS_SECTION_FROM_FILE tunerstudio/generated/temp/output_channels.ini \
 -readfile DATALOG_SECTION_FROM_FILE tunerstudio/generated/temp/data_logs.ini \
 -readfile LIVE_DATA_MENU_FROM_FILE tunerstudio/generated/temp/fancy_menu.ini \
 -readfile LIVE_DATA_PANELS_FROM_FILE tunerstudio/generated/temp/fancy_content.ini \
 -ts_destination tunerstudio \
 -firing_order controllers/algo/firing_order.h \
 -triggerInputFolder ../unit_tests \
 -with_c_defines false \
 -field_lookup_file generated/value_lookup_generated.cpp generated/value_lookup_generated.md \
 -java_destination ../java_console/models/src/main/java/com/rusefi/config/generated/Fields.java \
 -initialize_to_zero false \
 -prepend generated/total_live_data_generated.h \
 -signature tunerstudio/generated/signature_${SHORT_BOARDNAME}.txt \
 -signature_destination generated/signature_${SHORT_BOARDNAME}.h \
 -ts_output_name generated/${INI} \
 -prepend integration/rusefi_config_shared.txt \
 -prepend ${BOARD_DIR}/prepend.txt \
 -board ${BOARD_DIR} \
 -definition integration/rusefi_config.txt"
