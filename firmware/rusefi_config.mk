include $(PROJECT_DIR)/../java_tools/java_tools.mk

# We're assuming that META_OUTPUT_ROOT_FOLDER is a path relative to PROJECT_DIR
INI_FILE = $(PROJECT_DIR)/$(META_OUTPUT_ROOT_FOLDER)tunerstudio/generated/rusefi_$(SHORT_BOARD_NAME).ini
SIG_FILE = $(PROJECT_DIR)/$(META_OUTPUT_ROOT_FOLDER)tunerstudio/generated/signature_$(SHORT_BOARD_NAME).txt

CONFIG_INPUTS = \
  $(SIG_FILE) \
  $(PROJECT_DIR)/controllers/algo/rusefi_hw_stm32_enums.h \
  $(PROJECT_DIR)/controllers/algo/rusefi_hw_adc_enums.h \
  $(PROJECT_DIR)/integration/rusefi_config.txt \
  $(PROJECT_DIR)/tunerstudio/tunerstudio.template.ini \
  $(PROJECT_DIR)/${META_OUTPUT_ROOT_FOLDER}console/binary/generated/live_data_fragments.ini \
  $(PROJECT_DIR)/${META_OUTPUT_ROOT_FOLDER}console/binary/generated/data_logs.ini \
  $(PROJECT_DIR)/${META_OUTPUT_ROOT_FOLDER}console/binary/generated/fancy_content.ini \
  $(PROJECT_DIR)/${META_OUTPUT_ROOT_FOLDER}console/binary/generated/gauges.ini

ifneq ("$(wildcard $(BOARD_DIR)/prepend.txt)","")
  CONFIG_INPUTS += $(BOARD_DIR)/prepend.txt
endif

ifneq ("$(wildcard $(BOARD_DIR)/prepend_$(SHORT_BOARD_NAME).txt)","")
  CONFIG_INPUTS += $(BOARD_DIR)/prepend_$(SHORT_BOARD_NAME).txt
endif

ifneq ("$(wildcard $(BOARD_DIR)/board_options.ini)","")
  CONFIG_INPUTS += $(BOARD_DIR)/board_options.ini
endif

ifneq ("$(wildcard $(BOARD_DIR)/board_config.txt)","")
  CONFIG_INPUTS += $(BOARD_DIR)/board_config.txt
endif

# Build the generated pin code only if the connector directory exists
ifneq ("$(wildcard $(BOARD_DIR)/connectors)","")
  PIN_FILES = \
    $(PROJECT_DIR)/$(BOARD_DIR)/connectors/generated_outputs.h \
    $(PROJECT_DIR)/$(BOARD_DIR)/connectors/generated_ts_name_by_pin.cpp

  ALLCPPSRC += $(PROJECT_DIR)/$(BOARD_DIR)/connectors/generated_ts_name_by_pin.cpp
endif

RAMDISK = \
  $(PROJECT_DIR)/hw_layer/mass_storage/ramdisk_image.h \
  $(PROJECT_DIR)/hw_layer/mass_storage/ramdisk_image_compressed.h

CONFIG_FILES = \
  $(INI_FILE) \
  $(PROJECT_DIR)/$(META_OUTPUT_ROOT_FOLDER)controllers/generated/rusefi_generated_$(SHORT_BOARD_NAME).h \
  $(PROJECT_DIR)/$(META_OUTPUT_ROOT_FOLDER)controllers/generated/signature_$(SHORT_BOARD_NAME).h \
  $(PROJECT_DIR)/$(META_OUTPUT_ROOT_FOLDER)controllers/generated/engine_configuration_generated_structures_$(SHORT_BOARD_NAME).h \
  $(FIELDS) \
  $(PIN_FILES)

.FORCE:

# This is necessary because the ChibiOS makefile builds a .o file and generates
#  the deps for that .o file in the same GCC call, so if the .deps aren't already
#  in the correct state, things can fail to build because Make doesn't know it needs
#  to build the prerequisites (in this case CONFIG_FILES and RAMDISK) for those files ahead of time.
$(TCOBJS): $(CONFIG_FILES)
$(TCPPOBJS): $(RAMDISK)

# Always try to rebuild the signature file.
# The script won't actually update the file if the signature hasn't changed, so it won't trigger a config file generation.
$(SIG_FILE): .FORCE
	bash $(PROJECT_DIR)/gen_signature.sh $(SHORT_BOARD_NAME)

# Most sentinels are used for multiple targets that are created with a single recipe.
# In newer versions of GNU Make this is done using the &: operator,
#  but for supporting older versions, we do it this way.
# In particular, the version that ships with macOS is quite old.
$(RAMDISK): .ramdisk-sentinel ;

.ramdisk-sentinel: $(INI_FILE) $(TGT_SENTINEL)
	bash $(PROJECT_DIR)/bin/gen_image_board.sh $(BOARD_DIR) $(SHORT_BOARD_NAME)
	@touch $@

$(CONFIG_FILES): .config-sentinel ;

# CONFIG_DEFINITION is always rebuilt, but the file will only be updated if it needs to be,
# so it won't trigger a config file generation unless it needs to.
.config-sentinel: $(CONFIG_INPUTS) $(CONFIG_DEFINITION_JAR) $(TGT_SENTINEL)
ifneq (,$(CUSTOM_GEN_CONFIG))
	bash $(BOARD_DIR)/$(CUSTOM_GEN_CONFIG)
else
	bash $(PROJECT_DIR)/gen_config_board.sh $(BOARD_DIR) $(SHORT_BOARD_NAME)
endif
	@touch $@

.PHONY: config ini

ini: $(INI_FILE)

config: .config-sentinel
