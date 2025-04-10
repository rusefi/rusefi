# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration_uaefi121.cpp \
  $(BOARD_DIR)/mega-uaefi.cpp \


#no mux on mm100

# Add them all together
DDEFS += -DFIRMWARE_ID=\"uaefi121\" $(VAR_DEF_ENGINE_TYPE)
# EGT chip
DDEFS += -DEFI_MAX_31855=TRUE









include $(BOARD_DIR)/mega-uaefi.mk

DDEFS += $(PRIMARY_COMMUNICATION_PORT_USART2)
