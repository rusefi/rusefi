include $(PROJECT_DIR)/../java_tools/java_tools.mk

INI_FILE = $(PROJECT_DIR)/$(META_OUTPUT_ROOT_FOLDER)tunerstudio/generated/rusefi_$(SHORT_BOARD_NAME).ini
SIG_FILE = $(PROJECT_DIR)/tunerstudio/generated/signature_$(SHORT_BOARD_NAME).txt

CONFIG_INPUTS = \
  $(SIG_FILE) \
  $(PROJECT_DIR)/integration/rusefi_config.txt \
  $(PROJECT_DIR)/console/binary/generated/output_channels.ini \
  $(PROJECT_DIR)/console/binary/generated/data_logs.ini \
  $(PROJECT_DIR)/console/binary/generated/fancy_content.ini \
  $(PROJECT_DIR)/console/binary/generated/gauges.ini

# Build the generated pin code only if the connector directory exists
ifneq ("$(wildcard $(BOARD_DIR)/connectors)","")
  PIN_FILES = \
    $(PROJECT_DIR)/$(BOARD_DIR)/connectors/generated_outputs.h \
    $(PROJECT_DIR)/$(BOARD_DIR)/connectors/generated_ts_name_by_pin.cpp
endif

RAMDISK = \
  $(PROJECT_DIR)/hw_layer/mass_storage/ramdisk_image.h \
  $(PROJECT_DIR)/hw_layer/mass_storage/ramdisk_image_compressed.h

CONFIG_FILES = \
  $(INI_FILE) \
  $(PROJECT_DIR)/controllers/generated/rusefi_generated_$(SHORT_BOARD_NAME).h \
  $(PROJECT_DIR)/controllers/generated/signature_$(SHORT_BOARD_NAME).h \
  $(FIELDS) \
  $(PIN_FILES)

.FORCE:

$(TCOBJS): $(CONFIG_FILES)
$(TCPPOBJS): $(RAMDISK)

$(SIG_FILE): .FORCE
	bash $(PROJECT_DIR)/gen_signature.sh $(SHORT_BOARD_NAME)

$(RAMDISK): .ramdisk-sentinel ;

.ramdisk-sentinel: $(INI_FILE) .FORCE
	test $$(cat $@ 2>/dev/null) == $(SHORT_BOARD_NAME) -a \
	! $$(echo "$?" | cut -d ' ' -f 1) -ef $(INI_FILE) \
	|| (bash $(PROJECT_DIR)/bin/gen_image_board.sh $(BOARD_DIR) $(SHORT_BOARD_NAME) \
	&& echo $(SHORT_BOARD_NAME) >$@)

$(CONFIG_FILES): .config-sentinel ;

.config-sentinel: $(CONFIG_INPUTS) $(CONFIG_DEFINITION)
ifneq (,$(CUSTOM_GEN_CONFIG))
	bash $(BOARD_DIR)/$(CUSTOM_GEN_CONFIG)
else
	bash $(PROJECT_DIR)/gen_config_board.sh $(BOARD_DIR) $(SHORT_BOARD_NAME)
endif
	@touch $@
