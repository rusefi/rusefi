# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp

DDEFS += -DEFI_MAIN_RELAY_CONTROL=TRUE

# pretty temporary just to try https://github.com/rusefi/rusefi/issues/6079 on discovery
DDEFS += -DDISABLE_PIN_STATE_VALIDATION=TRUE

#no mux on mm100

# Add them all together
DDEFS += -DFIRMWARE_ID=\"uaefi\" $(VAR_DEF_ENGINE_TYPE)
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE
# EGT chip
DDEFS += -DEFI_MAX_31855=TRUE

DDEFS += -DHELLEN_BOARD_ID_DEBUG

DDEFS += -DHW_HELLEN_UAEFI=1
DDEFS += -DDIAG_5VP_PIN=Gpio::MM100_SPI3_MOSI

ONBOARD_MEMS_TYPE=LIS2DH12

include $(BOARDS_DIR)/hellen/hellen-common100.mk

DDEFS += $(PRIMARY_COMMUNICATION_PORT_USART2)

