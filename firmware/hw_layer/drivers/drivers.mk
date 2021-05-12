DRIVERS_DIR=$(PROJECT_DIR)/hw_layer/drivers

HW_LAYER_DRIVERS_INC = \
	$(DRIVERS_DIR) \
	$(DRIVERS_DIR)/gpio \
	$(DRIVERS_DIR)/can \
	$(DRIVERS_DIR)/serial \

HW_LAYER_DRIVERS_CORE = \
	$(DRIVERS_DIR)/gpio/core.c \

HW_LAYER_DRIVERS_CORE_CPP =

HW_LAYER_DRIVERS = \
	$(DRIVERS_DIR)/gpio/tle6240.c \
	$(DRIVERS_DIR)/gpio/tle8888.c \
	$(DRIVERS_DIR)/gpio/mc33972.c \
	$(DRIVERS_DIR)/gpio/mc33810.c \
	$(DRIVERS_DIR)/gpio/drv8860.c \

HW_LAYER_DRIVERS_CPP = \
	$(DRIVERS_DIR)/can/can_hw.cpp \
	$(DRIVERS_DIR)/can/can_msg_tx.cpp \
	$(DRIVERS_DIR)/serial/serial_hw.cpp
