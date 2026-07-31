BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp

# Hellen platform common bits: HW_HELLEN=1, hellen_common.cpp, board-id
# (inert here - the premium module has no board-id pads/pins defined), and
# LED_PIN_MODE=OM_INVERTED which matches this module's LED wiring
# (V33 -> resistor -> LED -> MCU pin, the pin sinks current)
include $(BOARDS_DIR)/hellen/hellen-common.mk

DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::MMP176_LED1_RED

# USB FS device on module pads USBM/USBP (PA11/PA12, AF10) - same as the
# STM32F7 port defaults, stated explicitly; module has no USB ID pad, and
# VBUS sensing is ignored (BOARD_OTG_NOVBUSSENS in the port board.h)
DDEFS += -DEFI_USB_SERIAL_DM=Gpio::A11
DDEFS += -DEFI_USB_SERIAL_DP=Gpio::A12
