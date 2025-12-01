
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

set_board_file BOARD_ENGINE_CONFIGURATION_FILE "${BOARD_DIR}/board_engine_configuration.txt"
set_board_file BOARD_CONFIG_FILE "${BOARD_DIR}/board_config.txt"
set_board_file BOARD_OPTIONS_FILE "${BOARD_DIR}/board_options.ini"
set_board_file BOARD_MENU_FILE "${BOARD_DIR}/board_menu.ini"
set_board_file BOARD_VE_MENU_FILE "${BOARD_DIR}/board_ve_menu.ini"
set_board_file BOARD_IGNITION_ADVANCE_MENU_FILE "${BOARD_DIR}/board_ignition_advance_menu.ini"
set_board_file BOARD_TABLES_FILE "${BOARD_DIR}/board_tables.ini"
set_board_file BOARD_CURVES_FILE "${BOARD_DIR}/board_curves.ini"
set_board_file BOARD_PANELS_FILE "${BOARD_DIR}/board_panels.ini"
set_board_file BOARD_CAN_BUS_FILE "${BOARD_DIR}/board_panel_can_main.ini"
set_board_file BOARD_PC_VARIABLES_FROM_FILE "${BOARD_DIR}/board_pc_variables.ini"
set_board_file BOARD_INDICATORS_FILE "${BOARD_DIR}/board_indicators.ini"
set_board_file BOARD_GAUGES_FILE "${BOARD_DIR}/board_gauges.ini"
set_board_file BOARD_CONSTANTS_EXTENSIONS_FILE "${BOARD_DIR}/board_constants_extensions.ini"
set_board_file BOARD_COMMANDS_FILE "${BOARD_DIR}/board_commands.ini"
set_board_file BOARD_POPULAR_VEHICLES_FILE "${BOARD_DIR}/board_popular_vehicles.ini"
set_board_file BOARD_ENGINE_METADATA_FILE "${BOARD_DIR}/board_engine_metadata.ini"
set_board_file BOARD_VIN_FIELD_FILE "${BOARD_DIR}/board_vin_field.ini"
set_board_file BOARD_INJECTOR_SETTINGS_FILE "${BOARD_DIR}/board_injector_settings.ini"
set_board_file BOARD_PRIMING_PULSE_PANEL_FILE "${BOARD_DIR}/board_priming_pulse_panel.ini"
set_board_file BOARD_CAM_SETTINGS_FILE "${BOARD_DIR}/board_cam_settings.ini"
set_board_file BOARD_DIAG_PANEL1_FILE "${BOARD_DIR}/board_diag_panel1.ini"
set_board_file BOARD_DIAG_PANEL2_FILE "${BOARD_DIR}/board_diag_panel2.ini"
set_board_file BOARD_VE_FILE "${BOARD_DIR}/board_ve.ini"
set_board_file BOARD_INJECTION_SETTINGS_FILE "${BOARD_DIR}/board_injection_settings.ini"
set_board_file BOARD_IGNITION_SETTINGS_FILE "${BOARD_DIR}/board_ignition_settings.ini"
set_board_file BOARD_FRONT_PAGE_FILE "${BOARD_DIR}/board_front_page.ini"


# most static arguments go first
# reading into VariableRegistry and overrides go next

COMMON_GEN_CONFIG="
 -initialize_to_zero false \
 -with_c_defines false \
 -prepend integration/ts_protocol.txt \
 -readfile OUTPUTS_SECTION_FROM_FILE ${META_OUTPUT_ROOT_FOLDER}console/binary/generated/live_data_fragments.ini \
 -readfile DATALOG_SECTION_FROM_FILE ${META_OUTPUT_ROOT_FOLDER}console/binary/generated/data_logs.ini \
 -readfile LIVE_DATA_MENU_FROM_FILE ${META_OUTPUT_ROOT_FOLDER}console/binary/generated/fancy_menu.ini \
 -readfile LIVE_DATA_PANELS_FROM_FILE ${META_OUTPUT_ROOT_FOLDER}console/binary/generated/fancy_content.ini \
 -readfile LIVE_DATA_GAUGES_FROM_FILE ${META_OUTPUT_ROOT_FOLDER}console/binary/generated/gauges.ini \
 -readfile BOARD_ENGINE_CONFIGURATION_FROM_FILE ${BOARD_ENGINE_CONFIGURATION_FILE} \
 -readfile BOARD_CONFIG_FROM_FILE ${BOARD_CONFIG_FILE} \
 -readfile BOARD_OPTIONS_FROM_FILE ${BOARD_OPTIONS_FILE} \
 -readfile BOARD_MENU_FROM_FILE ${BOARD_MENU_FILE} \
 -readfile BOARD_VE_MENU_FROM_FILE ${BOARD_VE_MENU_FILE} \
 -readfile BOARD_IGNITION_ADVANCE_MENU_FROM_FILE ${BOARD_IGNITION_ADVANCE_MENU_FILE} \
 -readfile BOARD_TABLES_FROM_FILE ${BOARD_TABLES_FILE} \
 -readfile BOARD_VIN_FIELD_FROM_FILE ${BOARD_VIN_FIELD_FILE} \
 -readfile BOARD_INJECTOR_SETTINGS_FROM_FILE ${BOARD_INJECTOR_SETTINGS_FILE} \
 -readfile BOARD_PRIMING_PULSE_PANEL_FROM_FILE ${BOARD_PRIMING_PULSE_PANEL_FILE} \
 -readfile BOARD_CURVES_FROM_FILE ${BOARD_CURVES_FILE} \
 -readfile BOARD_CONSTANTS_EXTENSIONS_FROM_FILE ${BOARD_CONSTANTS_EXTENSIONS_FILE} \
 -readfile BOARD_DIAG_PANEL1_FROM_FILE ${BOARD_DIAG_PANEL1_FILE} \
 -readfile BOARD_DIAG_PANEL2_FROM_FILE ${BOARD_DIAG_PANEL2_FILE} \
 -readfile BOARD_PANELS_FROM_FILE ${BOARD_PANELS_FILE} \
 -readfile BOARD_CAN_BUS_FROM_FILE ${BOARD_CAN_BUS_FILE} \
 -readfile BOARD_PC_VARIABLES_FROM_FILE ${BOARD_PC_VARIABLES_FROM_FILE} \
 -readfile BOARD_INDICATORS_FROM_FILE ${BOARD_INDICATORS_FILE} \
 -readfile BOARD_GAUGES_FROM_FILE ${BOARD_GAUGES_FILE} \
 -readfile BOARD_VE_FROM_FILE ${BOARD_VE_FILE} \
 -readfile BOARD_INJECTION_SETTINGS_FROM_FILE ${BOARD_INJECTION_SETTINGS_FILE} \
 -readfile BOARD_IGNITION_SETTINGS_FROM_FILE ${BOARD_IGNITION_SETTINGS_FILE} \
 -readfile BOARD_FRONT_PAGE_FROM_FILE ${BOARD_FRONT_PAGE_FILE} \
 -readfile COMMANDS_FROM_FILE ${BOARD_COMMANDS_FILE} \
 -readfile BOARD_POPULAR_VEHICLES_FILE ${BOARD_POPULAR_VEHICLES_FILE} \
 -readfile BOARD_ENGINE_METADATA_FILE ${BOARD_ENGINE_METADATA_FILE} \
 -readfile BOARD_CAM_SETTINGS_FILE ${BOARD_CAM_SETTINGS_FILE} \
 -prepend integration/rusefi_config_trigger.txt \
 -prepend ${META_OUTPUT_ROOT_FOLDER}console/binary/generated/total_live_data_generated.h \
 -soft_prepend ${BOARD_DIR}/prepend.txt \
 -soft_prepend ${BOARD_DIR}/prepend_${SHORT_BOARD_NAME}.txt \
 -prepend integration/rusefi_config_shared.txt \
 -prepend integration/fields_api.txt \
 -firing_order \
 controllers/algo/firing_order.h \
 -ts_destination tunerstudio \
 -triggerInputFolder ../unit_tests \
 -field_lookup_file controllers/lua/generated/value_lookup_generated.cpp controllers/lua/generated/value_lookup_generated.md \
 -java_destination ../java_console/models/src/main/java/com/rusefi/config/generated/ \
 -signature ${META_OUTPUT_ROOT_FOLDER}tunerstudio/generated/signature_${SHORT_BOARD_NAME}.txt \
 -signature_destination controllers/generated/signature_${SHORT_BOARD_NAME}.h \
 -ts_output_name generated/${INI} \
 -board ${BOARD_DIR}"
