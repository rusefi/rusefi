DRIVERS_DIR=$(PROJECT_DIR)/hw_layer/drivers

HW_LAYER_DRIVERS_INC = \
	$(DRIVERS_DIR) \
	$(DRIVERS_DIR)\gpio \

HW_LAYER_DRIVERS_CORE = \
	$(DRIVERS_DIR)/gpio/core.c \

HW_LAYER_DRIVERS = \
	$(DRIVERS_DIR)/gpio/tle6240.c \
	

HW_LAYER_DRIVERS_CPP = \
	$(DRIVERS_DIR)/gpio/tle8888.cpp \
	
