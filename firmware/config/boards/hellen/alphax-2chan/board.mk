# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp
DDEFS += -DEFI_MAIN_RELAY_CONTROL=TRUE

# This board has trigger scope hardware!
DDEFS += -DTRIGGER_SCOPE

export USE_OPENBLT=yes

# Add them all together
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE

# pretty temporary
DDEFS += -DDISABLE_PIN_STATE_VALIDATION=TRUE

ONBOARD_MEMS_TYPE=LIS2DH12

include $(BOARDS_DIR)/hellen/hellen-common144.mk

DDEFS += $(PRIMARY_COMMUNICATION_PORT_USART2)

DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_ALPHAX_2CHAN

# let 2chan start asap
DDEFS += -DBOOT_BACKDOOR_ENTRY_TIMEOUT_MS=0
