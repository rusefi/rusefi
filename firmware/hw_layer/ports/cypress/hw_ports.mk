ifeq ($(CYPRESS_CONTRIB),)
	CYPRESS_CONTRIB = $(CHIBIOS_CONTRIB)
endif

HW_PORTS_SRC = \
	$(HW_PORT_DIR)/serial_over_usb/usbconfig.c \
	$(HW_PORT_DIR)/serial_over_usb/usbconsole.c \
	$(HW_PORT_DIR)/flash_int.c

HW_PORTS_CPP = \
	$(HW_PORT_DIR)/mpu_util.cpp \
	$(HW_PORT_DIR)/cypress_pins.cpp \
	$(HW_PORT_DIR)/cypress_common.cpp \
	$(HW_PORT_DIR)/backup_ram.cpp \
	$(HW_LAYER_DIR)/trigger_input_adc.cpp \
	$(HW_LAYER_DIR)/microsecond_timer/microsecond_timer_gpt.cpp

HW_PORTS_INC = \
	$(HW_LAYER_DIR)/ports/cypress/serial_over_usb
