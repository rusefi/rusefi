INI_FILE = $(META_OUTPUT_ROOT_FOLDER)tunerstudio/generated/rusefi_$(SHORT_BOARD_NAME).ini

CONFIG_FILES = \
  $(PROJECT_DIR)/$(INI_FILE) \
  $(PROJECT_DIR)/controllers/generated/$(PROJECT)_generated_$(SHORT_BOARD_NAME).h \
  $(PROJECT_DIR)/controllers/generated/signature_$(SHORT_BOARD_NAME).h \
  $(PROJECT_DIR)/hw_layer/mass_storage/ramdisk_image.h \
  $(PROJECT_DIR)/hw_layer/mass_storage/ramdisk_image_compressed.h \
  $(PROJECT_DIR)/../java_console/models/src/main/java/com/rusefi/config/generated/Fields.java \
  $(PROJECT_DIR)/$(BOARD_DIR)/connectors/generated_outputs.h \
  $(PROJECT_DIR)/$(BOARD_DIR)/connectors/generated_ts_name_by_pin.cpp

.FORCE:

$(ACOBJS): $(CONFIG_FILES)

$(CONFIG_FILES): .config-sentinel ;

.config-sentinel: .FORCE
ifneq (,$(CUSTOM_GEN_CONFIG))
	bash $(PROJECT_DIR)/$(BOARD_DIR)/$(CUSTOM_GEN_CONFIG)
else
	bash $(PROJECT_DIR)/gen_config_board.sh $(BOARD_DIR) $(SHORT_BOARD_NAME)
endif
	@touch $@
