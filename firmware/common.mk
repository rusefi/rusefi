include $(PROJECT_DIR)/init/init.mk
include $(PROJECT_DIR)/util/util.mk
include $(PROJECT_DIR)/config/engines/engines.mk
include $(PROJECT_DIR)/console/console.mk
include $(PROJECT_DIR)/controllers/controllers.mk
include $(PROJECT_DIR)/controllers/lua/lua.mk
include $(PROJECT_DIR)/development/development.mk
include $(PROJECT_DIR)/hw_layer/hw_layer.mk
include $(PROJECT_DIR)/hw_layer/sensors/sensors.mk
include $(PROJECT_DIR)/hw_layer/drivers/drivers.mk

FW_CONFIG_DIR = $(PROJECT_DIR)/config

ifneq ($(BOARD_DIR),)
# FW customization hook file
ifneq ("$(wildcard ../firmware/$(BOARD_DIR)/fw_configuration.cpp)","")
	ALLCPPSRC += ../firmware/$(BOARD_DIR)/fw_configuration.cpp
else
	ALLCPPSRC += $(FW_CONFIG_DIR)/fw_default_configuration.cpp
endif

# Default tune for board?
ifneq ("$(wildcard ../firmware/$(BOARD_DIR)/default_tune.cpp)","")
	BOARDCPPSRC += ../firmware/$(BOARD_DIR)/default_tune.cpp
endif
else
	# default setup_custom_fw_overrides() even no BOARD_DIR is defined (unit tests)
	ALLCPPSRC += $(FW_CONFIG_DIR)/fw_default_configuration.cpp
endif


ALLCSRC += \
	$(UTILSRC)

ALLCPPSRC += \
	$(CONTROLLERS_CORE_SRC_CPP) \
	$(CONTROLLERS_MATH_SRC_CPP) \
	$(ENGINES_SRC_CPP) \
	$(CONTROLLERS_ALGO_SRC_CPP) \
	$(CONTROLLERS_SENSORS_SRC_CPP) \
	$(TUNERSTUDIO_SRC_CPP) \
	$(HW_LAYER_CPP) \
	$(HW_LAYER_PORT_CPP) \
	$(HW_SENSORS_CPP) \
	$(SYSTEMSRC_CPP) \
	$(UTILSRC_CPP) \
	$(TRIGGER_DECODERS_SRC_CPP) \
	$(TRIGGER_SRC_CPP) \
	$(CONTROLLERS_SRC_CPP) \
	$(INIT_SRC_CPP) \

# The headers from `$(BOARD_DIR)/generated/live_data_generated` folder are more preferable than the headers from
# `$(PROJECT_DIR)/live_data_generated` folder
ifneq ("$(wildcard $(BOARD_DIR)/board_unit_tests.mk)","")
	# technical debt: this should be in a shared location for all private or custom repos!
	LIVE_DATA_GENERATED_DIRS := $(BOARD_DIR)/generated/live_data_generated
endif
LIVE_DATA_GENERATED_DIRS += $(PROJECT_DIR)/live_data_generated

ALLINC += \
	$(FW_CONFIG_DIR) \
	$(CONSOLE_INC) \
 	$(DEVELOPMENT_DIR) \
	$(ENGINES_INC) \
	$(PROJECT_DIR)/config/engines \
	$(LIVE_DATA_GENERATED_DIRS) \
	$(BOARDS_DIR) \
	$(PROJECT_DIR)/hw_layer/algo \
    $(PROJECT_DIR)/init \
    $(PROJECT_DIR)/ext_algo \
	$(HW_LAYER_DRIVERS_INC) \
	$(HW_LAYER_INC) \
  	$(HW_SENSORS_INC) \
	$(UTIL_INC) \
	$(CONTROLLERS_SENSORS_INC) \
	$(CONTROLLERS_INC) \
	$(PROJECT_DIR)/console/binary/generated \

