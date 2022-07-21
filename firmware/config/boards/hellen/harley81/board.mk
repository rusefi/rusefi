# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARDS_DIR)/hellen/harley81/board_configuration.cpp
BOARDINC = $(BOARDS_DIR)/hellen/harley81

DDEFS += -DEFI_MAIN_RELAY_CONTROL=TRUE



# Add them all together
DDEFS += -DFIRMWARE_ID=\"hellen81hd\"
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE

DDEFS += -DSTM32_PWM_USE_TIM3=TRUE

include $(BOARDS_DIR)/hellen/hellen-common144.mk

# Enable serial pins on expansion header
DDEFS += $(PRIMARY_COMMUNICATION_PORT_USART2)

DDEFS += -DSHORT_BOARD_NAME=harley81

