# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration_uaefi121.cpp \
  $(BOARD_DIR)/mega-uaefi.cpp \


#no mux on mm100

# Add them all together
DDEFS += -DFIRMWARE_ID=\"uaefi121\" $(VAR_DEF_ENGINE_TYPE)
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE
# EGT chip
DDEFS += -DEFI_MAX_31855=TRUE



DDEFS += -DEFI_SKIP_BOR=TRUE




DDEFS += -DBOARD_SERIAL="\"000200000000000000000000\""

include $(BOARD_DIR)/mega-uaefi.mk

DDEFS += $(PRIMARY_COMMUNICATION_PORT_USART2)

DDEFS += -DUSB_DESCRIPTOR_STRING_CONTENT="'r', 0, 'u', 0, 's', 0, 'E', 0, 'F', 0, 'I', 0, ' ', 0, 'u', 0, 'a', 0, 'E', 0, 'F', 0, 'I', 0, '1', 0, '2', 0, '1', 0"
