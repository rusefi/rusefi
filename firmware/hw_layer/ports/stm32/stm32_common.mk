HW_PORTS_SRC = \
	$(HW_LAYER_DIR)/ports/stm32/serial_over_usb/usbcfg.c \
	$(HW_LAYER_DIR)/ports/stm32/serial_over_usb/usbconsole.c \
	$(HW_LAYER_DIR)/ports/stm32/flash_int.c

HW_PORTS_CPP = \
	$(HW_LAYER_DIR)/ports/stm32/stm32_pins.cpp \
	$(HW_LAYER_DIR)/ports/stm32/stm32_common.cpp \
	$(HW_LAYER_DIR)/ports/stm32/backup_ram.cpp \
	$(HW_LAYER_DIR)/ports/stm32/microsecond_timer_stm32.cpp \

RUSEFIASM = \
	$(HW_LAYER_DIR)/ports/stm32/rusEfiStartup.S

HW_PORTS_INC = \
	$(HW_LAYER_DIR)/ports/stm32 \
	$(HW_LAYER_DIR)/ports/stm32/serial_over_usb
