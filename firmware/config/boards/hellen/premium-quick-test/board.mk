BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp

# Hellen platform common bits: HW_HELLEN=1, hellen_common.cpp, board-id
# (inert here - the premium module has no board-id pads/pins defined), and
# LED_PIN_MODE=OM_INVERTED which matches this module's LED wiring
# (V33 -> resistor -> LED -> MCU pin, the pin sinks current)
include $(BOARDS_DIR)/hellen/hellen-common.mk

DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::MMP176_LED1_RED

# third bxCAN peripheral for the third on-module transceiver
# (EFI_CAN_BUS_COUNT=3 comes from prepend.txt via the generated config)
DDEFS += -DSTM32_CAN_USE_CAN3=TRUE

# AIN1..6 analog mux select = PH15 (MMP176_MUX_CONTROL; raw pin here because
# the ADC driver TU does not see hellen meta names). The slow ADC samples
# channels 0-15 with this pin LOW (AIN1/AIN4/AIN6) and 16-31 with it HIGH
# (AIN2/AIN3/AIN5), see stm32_adc_v2.cpp
DDEFS += -DADC_MUX_PIN=Gpio::H15

# USB FS device on module pads USBM/USBP (PA11/PA12, AF10) - same as the
# STM32F7 port defaults, stated explicitly; module has no USB ID pad, and
# VBUS sensing is ignored (BOARD_OTG_NOVBUSSENS in the port board.h)
DDEFS += -DEFI_USB_SERIAL_DM=Gpio::A11
DDEFS += -DEFI_USB_SERIAL_DP=Gpio::A12
