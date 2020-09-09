HW_LAYER_EMS += $(PROJECT_DIR)/hw_layer/ports/stm32/flash_int.c \
				$(PROJECT_DIR)/hw_layer/ports/stm32/stm32f4/stm32f4xx_hal_flash.c \
				$(PROJECT_DIR)/hw_layer/ports/stm32/stm32f4/stm32f4xx_hal_flash_ex.c \
                $(PROJECT_DIR)/hw_layer/ports/stm32/serial_over_usb/usbcfg.c \
	            $(PROJECT_DIR)/hw_layer/ports/stm32/serial_over_usb/usbconsole.c

HW_LAYER_EMS += $(PROJECT_DIR)/hw_layer/ports/stm32/flash.c \
				$(PROJECT_DIR)/hw_layer/ports/stm32/stm32f4/stm32f4xx_hal_flash.c \
				$(PROJECT_DIR)/hw_layer/ports/stm32/stm32f4/stm32f4xx_hal_flash_ex.c

HW_LAYER_EMS_CPP += $(PROJECT_DIR)/hw_layer/ports/stm32/stm32f4/mpu_util.cpp \
	$(PROJECT_DIR)/hw_layer/ports/stm32/stm32_pins.cpp \
	$(PROJECT_DIR)/hw_layer/ports/stm32/stm32_common.cpp \
	$(PROJECT_DIR)/hw_layer/ports/stm32/backup_ram.cpp

RUSEFIASM = $(PROJECT_DIR)/hw_layer/ports/stm32/rusEfiStartup.S

HW_INC += $(PROJECT_DIR)/hw_layer/ports/stm32 $(PROJECT_DIR)/hw_layer/ports/stm32/serial_over_usb
