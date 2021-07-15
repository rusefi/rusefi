HW_LAYER_EMS += $(PROJECT_DIR)/hw_layer/ports/stm32/serial_over_usb/usbcfg.c \
				$(PROJECT_DIR)/hw_layer/ports/stm32/flash_int.c \

HW_LAYER_EMS_CPP += \
    $(PROJECT_DIR)/hw_layer/ports/stm32/serial_over_usb/usbconsole.cpp \
	$(PROJECT_DIR)/hw_layer/ports/stm32/stm32_pins.cpp \
	$(PROJECT_DIR)/hw_layer/ports/stm32/stm32_common.cpp \
	$(PROJECT_DIR)/hw_layer/ports/stm32/backup_ram.cpp \
	$(PROJECT_DIR)/hw_layer/ports/stm32/microsecond_timer_stm32.cpp \
	$(PROJECT_DIR)/hw_layer/ports/stm32/osc_detector.cpp \

RUSEFIASM = $(PROJECT_DIR)/hw_layer/ports/stm32/rusEfiStartup.S

HW_INC += $(PROJECT_DIR)/hw_layer/ports/stm32 $(PROJECT_DIR)/hw_layer/ports/stm32/serial_over_usb
