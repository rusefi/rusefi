HW_LAYER_EGT = $(PROJECT_DIR)/hw_layer/serial_over_usb/usbcfg.c \
	$(PROJECT_DIR)/hw_layer/serial_over_usb/usbconsole.c

HW_LAYER_EGT_CPP = $(PROJECT_DIR)/hw_layer/can_hw.cpp \
	$(PROJECT_DIR)/hw_layer/max31855.cpp

HW_LAYER_EMS = $(HW_LAYER_EGT) \
	$(PROJECT_DIR)/hw_layer/mcp3208.c \
	$(PROJECT_DIR)/hw_layer/mc33816_data.c \

HW_LAYER_EMS_CPP = $(HW_LAYER_EGT_CPP) \
	$(PROJECT_DIR)/hw_layer/pin_repository.cpp \
	$(PROJECT_DIR)/hw_layer/microsecond_timer.cpp \
	$(PROJECT_DIR)/hw_layer/digital_input_hw.cpp \
	$(PROJECT_DIR)/hw_layer/digital_input_exti.cpp \
	$(PROJECT_DIR)/hw_layer/hardware.cpp \
	$(PROJECT_DIR)/hw_layer/smart_gpio.cpp \
	$(PROJECT_DIR)/hw_layer/neo6m.cpp \
	$(PROJECT_DIR)/hw_layer/mmc_card.cpp \
	$(PROJECT_DIR)/hw_layer/lcd/lcd_HD44780.cpp \
 	$(PROJECT_DIR)/hw_layer/adc_inputs.cpp \
	$(PROJECT_DIR)/hw_layer/board_test.cpp \
	$(PROJECT_DIR)/hw_layer/pwm_generator.cpp \
	$(PROJECT_DIR)/hw_layer/trigger_input.cpp \
	$(PROJECT_DIR)/hw_layer/hip9011.cpp \
	$(PROJECT_DIR)/hw_layer/mc33816.cpp \
	$(PROJECT_DIR)/hw_layer/hip9011_logic.cpp \
	$(PROJECT_DIR)/hw_layer/vehicle_speed.cpp \
	$(PROJECT_DIR)/hw_layer/stepper.cpp \
	$(PROJECT_DIR)/hw_layer/servo.cpp \
	$(PROJECT_DIR)/hw_layer/io_pins.cpp \
	$(PROJECT_DIR)/hw_layer/rtc_helper.cpp \
	$(PROJECT_DIR)/hw_layer/cdm_ion_sense.cpp \
	$(PROJECT_DIR)/hw_layer/backup_ram.cpp \

	
-include $(PROJECT_DIR)/hw_layer/$(CPU_HWLAYER)/hw_ports.mk
	