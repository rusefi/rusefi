HW_LAYER_EGT = $(PROJECT_DIR)/hw_layer/serial_over_usb/usbcfg.c \
	$(PROJECT_DIR)/hw_layer/serial_over_usb/usbconsole.c

HW_LAYER_EGT_CPP = $(PROJECT_DIR)/hw_layer/can_hw.cpp \
	$(PROJECT_DIR)/hw_layer/max31855.cpp

HW_LAYER_EMS = $(HW_LAYER_EGT) \
	$(PROJECT_DIR)/hw_layer/mcp3208.c \
	$(PROJECT_DIR)/hw_layer/flash.c \
	$(PROJECT_DIR)/hw_layer/stm32f4/stm32f4xx_hal_flash.c \
	$(PROJECT_DIR)/hw_layer/stm32f4/stm32f4xx_hal_flash_ex.c
		
HW_LAYER_EMS_CPP = $(HW_LAYER_EGT_CPP) \
	$(PROJECT_DIR)/hw_layer/accelerometer.cpp \
	$(PROJECT_DIR)/hw_layer/pin_repository.cpp \
	$(PROJECT_DIR)/hw_layer/microsecond_timer.cpp \
	$(PROJECT_DIR)/hw_layer/digital_input_hw.cpp \
	$(PROJECT_DIR)/hw_layer/hardware.cpp \
	$(PROJECT_DIR)/hw_layer/neo6m.cpp \
	$(PROJECT_DIR)/hw_layer/mmc_card.cpp \
	$(PROJECT_DIR)/hw_layer/lcd/lcd_HD44780.cpp \
 	$(PROJECT_DIR)/hw_layer/adc_inputs.cpp \
	$(PROJECT_DIR)/hw_layer/board_test.cpp \
	$(PROJECT_DIR)/hw_layer/pwm_generator.cpp \
	$(PROJECT_DIR)/hw_layer/trigger_input.cpp \
	$(PROJECT_DIR)/hw_layer/HIP9011.cpp \
	$(PROJECT_DIR)/hw_layer/vehicle_speed.cpp \
	$(PROJECT_DIR)/hw_layer/joystick.cpp \
	$(PROJECT_DIR)/hw_layer/stepper.cpp \
	$(PROJECT_DIR)/hw_layer/servo.cpp \
	$(PROJECT_DIR)/hw_layer/io_pins.cpp \
	$(PROJECT_DIR)/hw_layer/stm32f4/mpu_util.cpp \
	$(PROJECT_DIR)/hw_layer/rtc_helper.cpp \
	$(PROJECT_DIR)/hw_layer/backup_ram.cpp
	