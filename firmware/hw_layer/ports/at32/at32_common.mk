HW_AT32_PORT_DIR = $(PROJECT_DIR)/hw_layer/ports/at32

HW_LAYER_PORT_CPP += \
	$(HW_AT32_PORT_DIR)/at32_common.cpp \
	$(HW_AT32_PORT_DIR)/at32f4/mpu_util.cpp
