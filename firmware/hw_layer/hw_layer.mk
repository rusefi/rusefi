
HW_LAYER_INC=	$(PROJECT_DIR)/hw_layer $(PROJECT_DIR)/hw_layer/adc \
	$(PROJECT_DIR)/hw_layer/digital_input \
	$(PROJECT_DIR)/hw_layer/digital_input/trigger \
	$(PROJECT_DIR)/hw_layer/microsecond_timer \

HW_INC = hw_layer/$(CPU_HWLAYER) \
	$(PROJECT_DIR)/hw_layer/ports

HW_LAYER_EMS = $(HW_LAYER_EGT) \
	$(PROJECT_DIR)/hw_layer/adc/mcp3208.c \
	$(PROJECT_DIR)/hw_layer/mc33816_data.c

HW_LAYER_EMS_CPP = \
	$(PROJECT_DIR)/hw_layer/pin_repository.cpp \
	$(PROJECT_DIR)/hw_layer/microsecond_timer/microsecond_timer.cpp \
	$(PROJECT_DIR)/hw_layer/digital_input/digital_input.cpp \
	$(PROJECT_DIR)/hw_layer/digital_input/digital_input_icu.cpp \
	$(PROJECT_DIR)/hw_layer/digital_input/digital_input_exti.cpp \
	$(PROJECT_DIR)/hw_layer/digital_input/trigger/trigger_input.cpp \
	$(PROJECT_DIR)/hw_layer/digital_input/trigger/trigger_input_icu.cpp \
	$(PROJECT_DIR)/hw_layer/digital_input/trigger/trigger_input_exti.cpp \
	$(PROJECT_DIR)/hw_layer/hardware.cpp \
	$(PROJECT_DIR)/hw_layer/smart_gpio.cpp \
	$(PROJECT_DIR)/hw_layer/mmc_card.cpp \
	$(PROJECT_DIR)/hw_layer/adc/adc_inputs.cpp \
	$(PROJECT_DIR)/hw_layer/adc/adc_subscription.cpp \
	$(PROJECT_DIR)/hw_layer/adc/ads1015.cpp \
	$(PROJECT_DIR)/hw_layer/sensors/hip9011.cpp \
	$(PROJECT_DIR)/hw_layer/sensors/hip9011_logic.cpp \
	$(PROJECT_DIR)/hw_layer/mc33816.cpp \
	$(PROJECT_DIR)/hw_layer/stepper.cpp \
	$(PROJECT_DIR)/hw_layer/stepper_dual_hbridge.cpp \
	$(PROJECT_DIR)/hw_layer/servo.cpp \
	$(PROJECT_DIR)/hw_layer/io_pins.cpp \
	$(PROJECT_DIR)/hw_layer/rtc_helper.cpp \
	$(PROJECT_DIR)/hw_layer/cdm_ion_sense.cpp \
	$(PROJECT_DIR)/hw_layer/debounce.cpp \


#
# '-include' is a magic kind of 'include' which would survive if file to be included is not found
#	
-include $(PROJECT_DIR)/hw_layer/$(CPU_HWLAYER)/hw_ports.mk

