HW_LAYER_EMS_CPP += \
    $(PROJECT_DIR)/hw_layer/ports/stm32/serial_over_usb/usbconsole.cpp \
	$(PROJECT_DIR)/hw_layer/ports/stm32/stm32_pins.cpp \
	$(PROJECT_DIR)/hw_layer/ports/stm32/stm32_common.cpp \
	$(PROJECT_DIR)/hw_layer/ports/stm32/backup_ram.cpp \
	$(PROJECT_DIR)/hw_layer/ports/stm32/microsecond_timer_stm32.cpp \
	$(PROJECT_DIR)/hw_layer/ports/stm32/osc_detector.cpp \
	$(PROJECT_DIR)/hw_layer/ports/stm32/flash_int.cpp \
	$(PROJECT_DIR)/hw_layer/ports/stm32/serial_over_usb/usbcfg.cpp \


RUSEFIASM = $(PROJECT_DIR)/hw_layer/ports/stm32/rusEfiStartup.S

HW_INC += \
	$(PROJECT_DIR)/hw_layer/ports/stm32 \
	$(PROJECT_DIR)/hw_layer/ports/stm32/serial_over_usb

ifeq ($(EFI_HAS_EXT_SDRAM), yes)
	USE_OPT += -Wl,--defsym=STM32_HAS_SDRAM=1
	DDEFS += -DEFI_HAS_EXT_SDRAM
endif
