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
	
