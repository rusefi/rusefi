include $(PROJECT_DIR)/../java_tools/java_tools.mk

INI_FILE = $(META_OUTPUT_ROOT_FOLDER)tunerstudio/generated/rusefi_$(SHORT_BOARD_NAME).ini
SIG_FILE = $(PROJECT_DIR)/tunerstudio/generated/signature_$(SHORT_BOARD_NAME).txt

RAMDISK = \
  $(PROJECT_DIR)/hw_layer/mass_storage/ramdisk_image.h \
  $(PROJECT_DIR)/hw_layer/mass_storage/ramdisk_image_compressed.h

CONFIG_FILES = \
  $(PROJECT_DIR)/$(INI_FILE) \
  $(PROJECT_DIR)/controllers/generated/rusefi_generated_$(SHORT_BOARD_NAME).h \
  $(PROJECT_DIR)/controllers/generated/signature_$(SHORT_BOARD_NAME).h \
  $(PROJECT_DIR)/$(BOARD_DIR)/connectors/generated_outputs.h \
  $(PROJECT_DIR)/$(BOARD_DIR)/connectors/generated_ts_name_by_pin.cpp \
  $(FIELDS)

.FORCE:

$(TCOBJS): $(CONFIG_FILES)
mass_storage_init.o: $(RAMDISK)

$(SIG_FILE): .FORCE
	bash $(PROJECT_DIR)/gen_signature.sh $(SHORT_BOARD_NAME)

$(RAMDISK): .ramdisk-sentinel ;

.ramdisk-sentinel: $(INI_FILE)
	bash $(PROJECT_DIR)/bin/gen_image_board.sh $(BOARD_DIR) $(SHORT_BOARD_NAME)

$(CONFIG_FILES): .config-sentinel ;

.config-sentinel: $(SIG_FILE) .FORCE
ifneq (,$(CUSTOM_GEN_CONFIG))
	bash $(BOARD_DIR)/$(CUSTOM_GEN_CONFIG)
else
	bash $(PROJECT_DIR)/gen_config_board.sh $(BOARD_DIR) $(SHORT_BOARD_NAME)
endif
	@touch $@
