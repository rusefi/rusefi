DOCS_ENUMS_INPUTS = \
  $(PROJECT_DIR)/integration/rusefi_config.txt \
  $(PROJECT_DIR)/integration/LiveData.yaml \
  $(PROJECT_DIR)/console/binary/generated/live_data_ids.h \
  $(PROJECT_DIR)/controllers/sensors/sensor_type.h \
  $(PROJECT_DIR)/controllers/trigger/decoders/sync_edge.h \
  $(PROJECT_DIR)/controllers/algo/engine_types.h \
  $(PROJECT_DIR)/libfirmware/can/can_common.h \
  $(PROJECT_DIR)/controllers/algo/rusefi_enums.h \
  $(PROJECT_DIR)/hw_layer/drivers/can/can_category.h \
  $(PROJECT_DIR)/controllers/trigger/decoders/sync_edge.h \
  $(PROJECT_DIR)/controllers/algo/rusefi_hw_enums.h \
  $(PROJECT_DIR)/controllers/sensors/sensor_type.h \
  $(PROJECT_DIR)/config/boards/cypress/rusefi_hw_enums.h \
  $(PROJECT_DIR)/config/boards/kinetis/rusefi_hw_enums.h

.docsenums-sentinel: $(DOCS_ENUMS_INPUTS) $(CONFIG_DEFINITION_BASE) $(ENUM_TO_STRING)
	META_OUTPUT_ROOT_FOLDER="" bash $(PROJECT_DIR)/gen_live_documentation.sh
	@touch $@

.PHONY: docs-enums

docs-enums: .docsenums-sentinel

$(CONFIG_FILES): .docsenums-sentinel

# This is necessary because the ChibiOS makefile builds a .o file and generates
#  the deps for that .o file in the same GCC call, so if the .deps aren't already
#  in the correct state, things can fail to build because Make doesn't know it needs
#  to build the prerequisites (in this case the generated live docs and enums) for those files ahead of time.
$(OBJS): .docsenums-sentinel
