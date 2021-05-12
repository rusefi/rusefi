ifeq ($(CYPRESS_CONTRIB),)
  CYPRESS_CONTRIB = $(CHIBIOS_CONTRIB)
endif

HW_LAYER_EGT = $(PROJECT_DIR)/hw_layer/ports/cypress/serial_over_usb/usbconfig.c \
	$(PROJECT_DIR)/hw_layer/ports/cypress/serial_over_usb/usbconsole.c

HW_LAYER_EMS += 	$(PROJECT_DIR)/hw_layer/ports/cypress/flash_int.c

HW_LAYER_EMS_CPP += $(PROJECT_DIR)/hw_layer/ports/cypress/mpu_util.cpp \
	$(PROJECT_DIR)/hw_layer/ports/cypress/cypress_pins.cpp \
	$(PROJECT_DIR)/hw_layer/ports/cypress/cypress_common.cpp \
	$(PROJECT_DIR)/hw_layer/ports/cypress/backup_ram.cpp \
	$(PROJECT_DIR)/hw_layer/trigger_input_adc.cpp \
	$(PROJECT_DIR)/hw_layer/microsecond_timer/microsecond_timer_gpt.cpp \

HW_INC += $(PROJECT_DIR)/hw_layer/ports/cypress/serial_over_usb
