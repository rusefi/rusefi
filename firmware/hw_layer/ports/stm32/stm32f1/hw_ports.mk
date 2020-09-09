HW_LAYER_EGT = $(PROJECT_DIR)/hw_layer/ports/stm32/serial_over_usb/usbcfg.c \
	$(PROJECT_DIR)/hw_layer/ports/stm32/serial_over_usb/usbconsole.c

HW_LAYER_EMS += $(PROJECT_DIR)/hw_layer/ports/stm32/flash.c

HW_LAYER_EMS_CPP += $(PROJECT_DIR)/hw_layer/ports/stm32/stm32f1/mpu_util.cpp \
	$(PROJECT_DIR)/hw_layer/ports/stm32/stm32_pins.cpp \
	$(PROJECT_DIR)/hw_layer/ports/stm32/stm32_common.cpp \
	$(PROJECT_DIR)/hw_layer/ports/stm32/backup_ram.cpp
