
COMMON_GEN_CONFIG_PREFIX=" -DSystemOut.name=logs/gen_config_${SHORT_BOARD_NAME} \
 -jar ../java_tools/configuration_definition/build/libs/config_definition-all.jar"

set_board_file() {
    local var=$1
    local file_path=$2
    local default_file="tunerstudio/empty_board_options.ini"

    if [ ! -f "$file_path" ]; then
        eval "$var=\"$default_file\""
    else
        eval "$var=\"$file_path\""
    fi
}

set_board_file BOARD_CONFIG_FILE "${BOARD_DIR}/board_config.txt"
set_board_file BOARD_OPTIONS_FILE "${BOARD_DIR}/board_options.ini"
set_board_file BOARD_MENU_FILE "${BOARD_DIR}/board_menu.ini"
set_board_file BOARD_TABLES_FILE "${BOARD_DIR}/board_tables.ini"
set_board_file BOARD_PANELS_FILE "${BOARD_DIR}/board_panels.ini"

COMMON_GEN_CONFIG="
 -readfile OUTPUTS_SECTION_FROM_FILE console/binary/generated/output_channels.ini \
 -readfile DATALOG_SECTION_FROM_FILE console/binary/generated/data_logs.ini \
 -readfile LIVE_DATA_MENU_FROM_FILE console/binary/generated/fancy_menu.ini \
 -readfile LIVE_DATA_PANELS_FROM_FILE console/binary/generated/fancy_content.ini \
 -readfile LIVE_DATA_GAUGES_FROM_FILE console/binary/generated/gauges.ini \
 -readfile BOARD_CONFIG_FROM_FILE ${BOARD_CONFIG_FILE} \
 -readfile BOARD_OPTIONS_FROM_FILE ${BOARD_OPTIONS_FILE} \
 -readfile BOARD_MENU_FROM_FILE ${BOARD_MENU_FILE} \
 -readfile BOARD_TABLES_FROM_FILE ${BOARD_TABLES_FILE} \
 -readfile BOARD_PANELS_FROM_FILE ${BOARD_PANELS_FILE} \
  -ts_destination tunerstudio \
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
