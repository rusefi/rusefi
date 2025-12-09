# TODO: are we missing a ton of .txt file references from LiveData.yaml?!
DOCS_ENUMS_INPUTS = \
  $(PROJECT_DIR)/integration/rusefi_config.txt \
  $(PROJECT_DIR)/integration/config_page_1.txt \
  $(PROJECT_DIR)/integration/config_page_2.txt \
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
  $(PROJECT_DIR)/console/binary/generated/live_data_ids.h \
  $(PROJECT_DIR)/controllers/sensors/sensor_type.h \
  $(PROJECT_DIR)/controllers/trigger/decoders/sync_edge.h \
  $(PROJECT_DIR)/controllers/algo/engine_types.h \
  $(PROJECT_DIR)/controllers/algo/engine_type_e.h \
  $(PROJECT_DIR)/controllers/algo/rusefi_enums.h \
  $(PROJECT_DIR)/controllers/algo/rusefi_hw_enums.h \
  $(PROJECT_DIR)/libfirmware/can/can_common.h \
  $(PROJECT_DIR)/hw_layer/drivers/can/can_category.h \
  $(PROJECT_DIR)/config/boards/cypress/rusefi_hw_enums.h \
  $(PROJECT_DIR)/config/boards/kinetis/rusefi_hw_enums.h

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

$(CONFIG_FILES): .docsenums-sentinel

# This is necessary because the ChibiOS makefile builds a .o file and generates
#  the deps for that .o file in the same GCC call, so if the .deps aren't already
#  in the correct state, things can fail to build because Make doesn't know it needs
#  to build the prerequisites (in this case the generated live docs and enums) for those files ahead of time.
$(OBJS): .docsenums-sentinel
