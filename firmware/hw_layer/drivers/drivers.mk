DRIVERS_DIR=$(PROJECT_DIR)/hw_layer/drivers

HW_LAYER_DRIVERS_INC = \
	$(DRIVERS_DIR) \
	$(DRIVERS_DIR)/gpio \
	$(DRIVERS_DIR)/can \
	$(DRIVERS_DIR)/serial \

HW_LAYER_DRIVERS_CORE = \

HW_LAYER_DRIVERS_CORE_CPP = \
	$(DRIVERS_DIR)/gpio/core.cpp \

HW_LAYER_DRIVERS =

HW_LAYER_DRIVERS_CPP = \
	$(DRIVERS_DIR)/can/can_hw.cpp \
	$(DRIVERS_DIR)/can/can_msg_tx.cpp \
	$(DRIVERS_DIR)/serial/serial_hw.cpp \
	$(DRIVERS_DIR)/gpio/tle6240.cpp \
	$(DRIVERS_DIR)/gpio/tle8888.cpp \
	$(DRIVERS_DIR)/gpio/mc33972.cpp \
	$(DRIVERS_DIR)/gpio/mc33810.cpp \
	$(DRIVERS_DIR)/gpio/drv8860.cpp \
