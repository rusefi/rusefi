HW_LAYER_DIR=$(PROJECT_DIR)/hw_layer

include $(HW_LAYER_DIR)/drivers/drivers.mk
include $(HW_LAYER_DIR)/sensors/sensors.mk
include $(HW_LAYER_DIR)/mass_storage/mass_storage.mk

# this var is used in bootloader
HW_INC = \
	$(HW_LAYER_DIR)/$(CPU_HWLAYER) \
	$(HW_LAYER_DIR)/ports

HW_LAYER_INC = \
	$(HW_INC) \
	$(HW_LAYER_DRIVERS_INC) \
	$(HW_SENSORS_INC) \
	$(HW_LAYER_DIR) \
	$(HW_LAYER_DIR)/adc \
	$(HW_LAYER_DIR)/algo \
	$(HW_LAYER_DIR)/drivers/can \
	$(HW_LAYER_DIR)/digital_input \
	$(HW_LAYER_DIR)/digital_input/trigger \
	$(HW_LAYER_DIR)/lcd \
	$(HW_LAYER_DIR)/mass_storage \
	$(HW_LAYER_DIR)/microsecond_timer

HW_LAYER_SRC = \
	$(HW_LAYER_DRIVERS_CORE_SRC) \
	$(HW_LAYER_DRIVERS_SRC) \
	$(HW_SENSORS_SRC) \
	$(HW_LAYER_EGT) \
	$(HW_LAYER_DIR)/adc/mcp3208.c \
	$(HW_LAYER_DIR)/mc33816_data.c

HW_LAYER_CPP = \
	$(HW_LAYER_DRIVERS_CORE_CPP) \
	$(HW_LAYER_DRIVERS_CPP) \
	$(HW_SENSORS_CPP) \
	$(HW_LAYER_DIR)/pin_repository.cpp \
	$(HW_LAYER_DIR)/microsecond_timer/microsecond_timer.cpp \
	$(HW_LAYER_DIR)/digital_input/digital_input.cpp \
	$(HW_LAYER_DIR)/digital_input/digital_input_icu.cpp \
	$(HW_LAYER_DIR)/digital_input/digital_input_exti.cpp \
	$(HW_LAYER_DIR)/digital_input/trigger/trigger_input.cpp \
	$(HW_LAYER_DIR)/digital_input/trigger/trigger_input_icu.cpp \
	$(HW_LAYER_DIR)/digital_input/trigger/trigger_input_exti.cpp \
	$(HW_LAYER_DIR)/hardware.cpp \
	$(HW_LAYER_DIR)/smart_gpio.cpp \
	$(HW_LAYER_DIR)/mmc_card.cpp \
	$(HW_LAYER_DIR)/mmc_card_access.cpp \
	$(HW_LAYER_DIR)/adc/adc_inputs.cpp \
	$(HW_LAYER_DIR)/adc/adc_subscription.cpp \
	$(HW_LAYER_DIR)/adc/ads1015.cpp \
	$(HW_LAYER_DIR)/sensors/hip9011.cpp \
	$(HW_LAYER_DIR)/sensors/hip9011_logic.cpp \
	$(HW_LAYER_DIR)/mc33816.cpp \
	$(HW_LAYER_DIR)/vehicle_speed.cpp \
	$(HW_LAYER_DIR)/stepper.cpp \
	$(HW_LAYER_DIR)/stepper_dual_hbridge.cpp \
	$(HW_LAYER_DIR)/servo.cpp \
	$(HW_LAYER_DIR)/io_pins.cpp \
	$(HW_LAYER_DIR)/rtc_helper.cpp \
	$(HW_LAYER_DIR)/cdm_ion_sense.cpp \
	$(HW_LAYER_DIR)/debounce.cpp \

#
# '-include' is a magic kind of 'include' which would survive if file to be included is not found
#	
-include $(HW_LAYER_DIR)/$(CPU_HWLAYER)/hw_ports.mk
