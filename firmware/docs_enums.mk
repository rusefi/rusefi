# TODO: are we missing a ton of .txt file references from LiveData.yaml?!
DOCS_ENUMS_INPUTS = \
  $(PROJECT_DIR)/integration/rusefi_config.txt \
  $(PROJECT_DIR)/integration/rusefi_config_shared.txt \
  $(PROJECT_DIR)/integration/config_page_1.txt \
  $(PROJECT_DIR)/integration/config_page_2.txt \
  $(PROJECT_DIR)/integration/config_page_3.txt \
  $(PROJECT_DIR)/console/binary/output_channels.txt \
  $(PROJECT_DIR)/controllers/engine_cycle/knock_controller.txt \
  $(PROJECT_DIR)/controllers/engine_cycle/prime_injection.txt \
  $(PROJECT_DIR)/controllers/trigger/trigger_central.txt \
  $(PROJECT_DIR)/controllers/long_term_fuel_trim_state.txt \
  $(PROJECT_DIR)/controllers/math/short_term_fuel_trim_state.txt \
  $(PROJECT_DIR)/controllers/trigger/trigger_state.txt \
  $(PROJECT_DIR)/controllers/algo/shift_torque_reduction_state.txt \
  $(PROJECT_DIR)/controllers/engine_cycle/high_pressure_fuel_pump.txt \
  $(PROJECT_DIR)/controllers/actuators/idle_state.txt \
  $(PROJECT_DIR)/controllers/actuators/electronic_throttle.txt \
  $(PROJECT_DIR)/hw_layer/drivers/gpio/mc33810_state.txt \
  $(PROJECT_DIR)/integration/LiveData.yaml \
  $(PROJECT_DIR)/controllers/sensors/sensor_type.h \
  $(PROJECT_DIR)/controllers/trigger/decoders/sync_edge.h \
  $(PROJECT_DIR)/controllers/algo/engine_types.h \
  $(PROJECT_DIR)/controllers/algo/engine_type_e.h \
  $(PROJECT_DIR)/controllers/algo/rusefi_enums.h \
  $(PROJECT_DIR)/controllers/algo/rusefi_hw_enums.h \
  $(PROJECT_DIR)/libfirmware/can/can_common.h \
  $(PROJECT_DIR)/hw_layer/drivers/can/can_category.h \
  $(PROJECT_DIR)/config/boards/cypress/rusefi_hw_enums.h \
  $(PROJECT_DIR)/config/boards/kinetis/rusefi_hw_enums.h \

DOCS_ENUMS = \
  $(PROJECT_DIR)/controllers/algo/auto_generated_engine_type_e.cpp \
  $(PROJECT_DIR)/controllers/algo/auto_generated_engine_type_e.h \
  $(PROJECT_DIR)/controllers/algo/auto_generated_commonenum.cpp \
  $(PROJECT_DIR)/controllers/algo/auto_generated_commonenum.h \
  $(PROJECT_DIR)/$(META_OUTPUT_ROOT_FOLDER)controllers/algo/auto_generated_enums.cpp \
  $(PROJECT_DIR)/$(META_OUTPUT_ROOT_FOLDER)controllers/algo/auto_generated_enums.h \
  $(PROJECT_DIR)/controllers/algo/auto_generated_enginetypes.cpp \
  $(PROJECT_DIR)/controllers/algo/auto_generated_enginetypes.h \
  $(PROJECT_DIR)/controllers/trigger/decoders/auto_generated_sync_edge.cpp \
  $(PROJECT_DIR)/controllers/trigger/decoders/auto_generated_sync_edge.h \
  $(PROJECT_DIR)/controllers/sensors/auto_generated_sensor.cpp \
  $(PROJECT_DIR)/controllers/sensors/auto_generated_sensor.h \
  $(PROJECT_DIR)/hw_layer/drivers/can/auto_generated_can_category.cpp \
  $(PROJECT_DIR)/hw_layer/drivers/can/auto_generated_can_category.h \
  $(PROJECT_DIR)/$(META_OUTPUT_ROOT_FOLDER)controllers/lua/generated/output_lookup_generated.cpp \
  $(PROJECT_DIR)/$(META_OUTPUT_ROOT_FOLDER)console/binary/generated/data_logs.ini \
  $(PROJECT_DIR)/$(META_OUTPUT_ROOT_FOLDER)console/binary/generated/fancy_content.ini \
  $(PROJECT_DIR)/$(META_OUTPUT_ROOT_FOLDER)console/binary/generated/fancy_menu.ini \
  $(PROJECT_DIR)/$(META_OUTPUT_ROOT_FOLDER)console/binary/generated/gauges.ini \
  $(PROJECT_DIR)/$(META_OUTPUT_ROOT_FOLDER)console/binary/generated/live_data_fragments.h \
  $(PROJECT_DIR)/$(META_OUTPUT_ROOT_FOLDER)console/binary/generated/live_data_fragments.ini \
  $(PROJECT_DIR)/$(META_OUTPUT_ROOT_FOLDER)console/binary/generated/live_data_ids.h \
  $(PROJECT_DIR)/$(META_OUTPUT_ROOT_FOLDER)console/binary/generated/log_fields_generated.h \
  $(PROJECT_DIR)/$(META_OUTPUT_ROOT_FOLDER)console/binary/generated/sensors.java \
  $(PROJECT_DIR)/$(META_OUTPUT_ROOT_FOLDER)console/binary/generated/total_live_data_generated.h \
  $(PROJECT_DIR)/../java_tools/trigger-image/src/main/java/com/rusefi/config/generated/TriggerVariableRegistryValues.java \
  $(PROJECT_DIR)/../java_console/io/src/main/java/com/rusefi/enums/SensorType.java \
  $(PROJECT_DIR)/../java_console/io/src/main/java/com/rusefi/enums/StateDictionaryFactory.java \
  $(PROJECT_DIR)/../java_console/io/src/main/java/com/rusefi/enums/SyncEdge.java \
  $(PROJECT_DIR)/../java_console/io/src/main/java/com/rusefi/enums/live_data_e.java \
  $(PROJECT_DIR)/../java_console/models/src/main/java/com/rusefi/config/generated/Integration.java \
  $(PROJECT_DIR)/../java_console/models/src/main/java/com/rusefi/enums/bench_mode_e.java \
  $(PROJECT_DIR)/../java_console/models/src/main/java/com/rusefi/enums/bench_test_io_control_e.java \
  $(PROJECT_DIR)/../java_console/models/src/main/java/com/rusefi/enums/bench_test_magic_numbers_e.java \
  $(PROJECT_DIR)/../java_console/models/src/main/java/com/rusefi/enums/bench_test_packet_ids_e.java \
  $(PROJECT_DIR)/../java_console/models/src/main/java/com/rusefi/enums/debug_mode_e.java \
  $(PROJECT_DIR)/../java_console/models/src/main/java/com/rusefi/enums/engine_type_e.java \
  $(PROJECT_DIR)/../java_console/models/src/main/java/com/rusefi/enums/trigger_type_e.java \
  $(PROJECT_DIR)/../java_console/models/src/main/java/com/rusefi/enums/ts_14_command.java \
  $(PROJECT_DIR)/../java_console/models/src/main/java/com/rusefi/enums/ts_command_e.java \
  $(PROJECT_DIR)/../java_console/models/src/main/java/com/rusefi/tracing/EnumNames.java

ifneq ("$(wildcard $(BOARD_DIR)/extra.txt)","")
  DOCS_ENUMS_INPUTS += $(BOARD_DIR)/extra.txt
endif

ifneq ("$(wildcard $(BOARD_DIR)/firmware/extra.txt)","")
  DOCS_ENUMS_INPUTS += $(BOARD_DIR)/firmware/extra.txt
endif

ifneq ("$(wildcard $(BOARD_DIR)/board_config.txt)","")
  DOCS_ENUMS_INPUTS += $(BOARD_DIR)/board_config.txt
endif

.docsenums-sentinel: $(DOCS_ENUMS_INPUTS) $(CONFIG_DEFINITION_BASE_JAR) $(ENUM_TO_STRING_JAR)
	bash $(PROJECT_DIR)/bin/detect_github.sh
	bash $(PROJECT_DIR)/gen_live_documentation.sh
	@touch $@

.PHONY: docs-enums

docs-enums: .docsenums-sentinel

$(DOCS_ENUMS): .docsenums-sentinel

# This is necessary because the ChibiOS makefile builds a .o file and generates
#  the deps for that .o file in the same GCC call, so if the .deps aren't already
#  in the correct state, things can fail to build because Make doesn't know it needs
#  to build the prerequisites (in this case the generated live docs and enums) for those files ahead of time.
$(OBJS): .docsenums-sentinel
