
HW_LAYER_EMS = $(PROJECT_DIR)/hw_layer/pin_repository.c \
	$(PROJECT_DIR)/hw_layer/io_pins.c \
	$(PROJECT_DIR)/hw_layer/mcp3208.c \
	$(PROJECT_DIR)/hw_layer/lcd/lcd_HD44780.c \
	$(PROJECT_DIR)/hw_layer/microsecond_timer.c \
	$(PROJECT_DIR)/hw_layer/serial_over_usb/usbcfg.c \
	$(PROJECT_DIR)/hw_layer/serial_over_usb/usbconsole.c \
	$(PROJECT_DIR)/hw_layer/flash.c \
	$(PROJECT_DIR)/hw_layer/rtc_helper.c \
	$(PROJECT_DIR)/hw_layer/mmc_card.c \
	$(PROJECT_DIR)/hw_layer/neo6m.c \
	$(PROJECT_DIR)/hw_layer/gpio_helper.c \
	$(PROJECT_DIR)/hw_layer/wave_analyzer_hw.c

HW_LAYER_EMS_CPP = $(PROJECT_DIR)/hw_layer/hardware.cpp \
	$(PROJECT_DIR)/hw_layer/can_hw.cpp \
 	$(PROJECT_DIR)/hw_layer/adc_inputs.cpp \
	$(PROJECT_DIR)/hw_layer/board_test.cpp \
	$(PROJECT_DIR)/hw_layer/pwm_generator.cpp \
	$(PROJECT_DIR)/hw_layer/trigger_input.cpp \
	$(PROJECT_DIR)/hw_layer/HIP9011.cpp \
	$(PROJECT_DIR)/hw_layer/vehicle_speed.cpp \
	$(PROJECT_DIR)/hw_layer/max31855.cpp \
	$(PROJECT_DIR)/hw_layer/joystick.cpp \
	$(PROJECT_DIR)/hw_layer/stepper.cpp \
	$(PROJECT_DIR)/hw_layer/stm32f4/mpu_util.cpp

HW_LAYER_EGT_CPP = $(PROJECT_DIR)/hw_layer/can_hw.cpp \
	$(PROJECT_DIR)/hw_layer/max31855.cpp
	