include ../java_tools/java_tools.mk

INI_FILE = $(META_OUTPUT_ROOT_FOLDER)tunerstudio/generated/rusefi_$(SHORT_BOARD_NAME).ini
SIG_FILE = $(PROJECT_DIR)/tunerstudio/generated/signature_$(SHORT_BOARD_NAME).txt

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

$(TCOBJS): $(CONFIG_FILES)

$(SIG_FILE): .FORCE
	bash $(PROJECT_DIR)/gen_signature.sh $(SHORT_BOARD_NAME)

$(CONFIG_FILES): .config-sentinel ;

.config-sentinel: $(SIG_FILE) .FORCE
ifneq (,$(CUSTOM_GEN_CONFIG))
	bash $(PROJECT_DIR)/$(BOARD_DIR)/$(CUSTOM_GEN_CONFIG)
else
	bash $(PROJECT_DIR)/gen_config_board.sh $(BOARD_DIR) $(SHORT_BOARD_NAME)
endif
	@touch $@
