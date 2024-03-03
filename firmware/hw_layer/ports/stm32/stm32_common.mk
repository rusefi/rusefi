HW_STM32_PORT_DIR = $(PROJECT_DIR)/hw_layer/ports/stm32

HW_LAYER_PORT_CPP += \
    $(HW_STM32_PORT_DIR)/serial_over_usb/usbconsole.cpp \
	$(HW_STM32_PORT_DIR)/stm32_pins.cpp \
	$(HW_STM32_PORT_DIR)/stm32_common.cpp \
	$(HW_STM32_PORT_DIR)/stm32_adc.cpp \
	$(HW_STM32_PORT_DIR)/stm32_can.cpp \
	$(HW_STM32_PORT_DIR)/stm32_pwm.cpp \
	$(HW_STM32_PORT_DIR)/stm32_serial.cpp \
	$(HW_STM32_PORT_DIR)/stm32_spi.cpp \
	$(HW_STM32_PORT_DIR)/stm32_icu.cpp \
	$(HW_STM32_PORT_DIR)/stm32_reset_cause.cpp \
	$(HW_STM32_PORT_DIR)/backup_ram.cpp \
	$(HW_STM32_PORT_DIR)/microsecond_timer_stm32.cpp \
	$(HW_STM32_PORT_DIR)/osc_detector.cpp \
	$(HW_STM32_PORT_DIR)/flash_int.cpp \
	$(HW_STM32_PORT_DIR)/serial_over_usb/usbcfg.cpp

RUSEFIASM = \
	$(HW_STM32_PORT_DIR)/rusEfiStartup.S

HW_INC += \
	$(HW_STM32_PORT_DIR) \
	$(HW_STM32_PORT_DIR)/cfg \
	$(HW_STM32_PORT_DIR)/serial_over_usb

ifeq ($(EFI_HAS_EXT_SDRAM), yes)
	USE_OPT += -Wl,--defsym=STM32_HAS_SDRAM=1
	DDEFS += -DEFI_HAS_EXT_SDRAM
endif
