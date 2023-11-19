HW_AT32_PORT_DIR = $(PROJECT_DIR)/hw_layer/ports/at32

HW_LAYER_PORT_CPP += \
	$(HW_AT32_PORT_DIR)/at32_common.cpp \
	$(HW_AT32_PORT_DIR)/at32_can.cpp \
	$(HW_AT32_PORT_DIR)/at32_spi.cpp \
	$(HW_AT32_PORT_DIR)/at32f4/mpu_util.cpp

HW_INC += \
	$(HW_AT32_PORT_DIR)

#
# Use some stuff from STM32
#

HW_STM32_PORT_DIR = $(PROJECT_DIR)/hw_layer/ports/stm32

HW_LAYER_PORT_CPP += \
	$(HW_STM32_PORT_DIR)/serial_over_usb/usbconsole.cpp \
	$(HW_STM32_PORT_DIR)/serial_over_usb/usbcfg.cpp \
	$(HW_STM32_PORT_DIR)/stm32_pins.cpp \
	$(HW_STM32_PORT_DIR)/stm32_adc.cpp \
	$(HW_STM32_PORT_DIR)/stm32_pwm.cpp \
	$(HW_STM32_PORT_DIR)/stm32_serial.cpp \
	$(HW_STM32_PORT_DIR)/stm32_icu.cpp \
	$(HW_STM32_PORT_DIR)/microsecond_timer_stm32.cpp

#RUSEFIASM = \
#	$(HW_STM32_PORT_DIR)/rusEfiStartup.S

HW_STM32_PORT_DIR = $(PROJECT_DIR)/hw_layer/ports/stm32

HW_INC += \
	$(HW_STM32_PORT_DIR) \
	$(HW_STM32_PORT_DIR)/serial_over_usb
