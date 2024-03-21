
COMMON_GEN_CONFIG_PREFIX=" -DSystemOut.name=logs/gen_config_${SHORT_BOARD_NAME} \
 -jar ../java_tools/configuration_definition/build/libs/config_definition-all.jar"

COMMON_GEN_CONFIG="
 -readfile OUTPUTS_SECTION_FROM_FILE console/binary/generated/output_channels.ini \
 -readfile DATALOG_SECTION_FROM_FILE console/binary/generated/data_logs.ini \
 -readfile LIVE_DATA_MENU_FROM_FILE console/binary/generated/fancy_menu.ini \
 -readfile LIVE_DATA_PANELS_FROM_FILE console/binary/generated/fancy_content.ini \
 -readfile LIVE_DATA_GAUGES_FROM_FILE console/binary/generated/gauges.ini \
 -ts_template tunerstudio/rusefi.input \
 -triggerInputFolder ../unit_tests \
 -with_c_defines false \
 -field_lookup_file controllers/lua/generated/value_lookup_generated.cpp controllers/lua/generated/value_lookup_generated.md \
 -java_destination ../java_console/models/src/main/java/com/rusefi/config/generated/Fields.java \
 -initialize_to_zero false \
 -signature tunerstudio/generated/signature_${SHORT_BOARD_NAME}.txt \
 -signature_destination controllers/generated/signature_${SHORT_BOARD_NAME}.h \
 -ts_output_name generated/${INI} \
 -prepend integration/rusefi_config_trigger.txt \
 -prepend ${BOARD_DIR}/prepend.txt \
 -board ${BOARD_DIR}"
