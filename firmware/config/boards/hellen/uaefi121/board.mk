# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration_uaefi121.cpp



#no mux on mm100

# Add them all together
DDEFS += -DFIRMWARE_ID=\"uaefi121\" $(VAR_DEF_ENGINE_TYPE)
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE
# EGT chip
DDEFS += -DEFI_MAX_31855=TRUE









DDEFS += -DHW_HELLEN_UAEFI121=1


ONBOARD_MEMS_TYPE=LIS2DH12

include $(BOARDS_DIR)/hellen/hellen-common100.mk

DDEFS += $(PRIMARY_COMMUNICATION_PORT_USART2)
