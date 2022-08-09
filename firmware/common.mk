include $(PROJECT_DIR)/init/init.mk
include $(PROJECT_DIR)/util/util.mk
include $(PROJECT_DIR)/config/engines/engines.mk
include $(PROJECT_DIR)/console/console.mk
include $(PROJECT_DIR)/controllers/lua/lua.mk
include $(PROJECT_DIR)/controllers/controllers.mk
include $(PROJECT_DIR)/development/development.mk
include $(PROJECT_DIR)/hw_layer/hw_layer.mk
include $(PROJECT_DIR)/hw_layer/sensors/sensors.mk
include $(PROJECT_DIR)/hw_layer/drivers/drivers.mk

ALLCSRC += \
	$(UTILSRC)

ALLCPPSRC += \
	$(CONTROLLERS_CORE_SRC_CPP) \
	$(CONTROLLERS_MATH_SRC_CPP) \
	$(ENGINES_SRC_CPP) \
	$(CONTROLLERS_ALGO_SRC_CPP) \
	$(CONTROLLERS_SENSORS_SRC_CPP) \
	$(TUNERSTUDIO_SRC_CPP) \
	$(HW_LAYER_EMS_CPP) \
	$(HW_SENSORS_CPP) \
	$(SYSTEMSRC_CPP) \
	$(UTILSRC_CPP) \
	$(TRIGGER_DECODERS_SRC_CPP) \
	$(TRIGGER_SRC_CPP) \
	$(CONTROLLERS_SRC_CPP) \
	$(INIT_SRC_CPP) \


ALLINC += \
	$(CONSOLE_INC) \
 	$(DEVELOPMENT_DIR) \
	$(PROJECT_DIR)/config/engines \
	$(PROJECT_DIR)/config/boards/ \
	$(PROJECT_DIR)/hw_layer/algo \
    $(PROJECT_DIR)/init \
    $(PROJECT_DIR)/ext_algo \
	$(HW_LAYER_DRIVERS_INC) \
	$(HW_LAYER_INC) \
  	$(HW_SENSORS_INC) \
	$(UTIL_INC) \
	$(CONTROLLERS_INC) \
	$(PROJECT_DIR)/console/binary/generated \
	
