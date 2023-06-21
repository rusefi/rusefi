# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp
DDEFS += -DEFI_MAIN_RELAY_CONTROL=TRUE

# Turn off stuff we don't have/need
DDEFS += -DBOARD_TLE8888_COUNT=0

# Add them all together
DDEFS += -DFIRMWARE_ID=\"AlphaX-4chan\"
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE

DDEFS += -DADC_MUX_PIN=Gpio::F2

# pretty temporary
DDEFS += -DDISABLE_PIN_STATE_VALIDATION=TRUE

include $(BOARDS_DIR)/hellen/hellen-common144.mk

ifeq ($(PROJECT_CPU),ARCH_STM32F7)
	DDEFS += -DSHORT_BOARD_NAME=alphax-4chan-f7
	# TODO: why do I struggle to fit into flash? compare with Proteus
	DDEFS += -DCH_DBG_ENABLE_ASSERTS=FALSE
else ifeq ($(PROJECT_CPU),ARCH_STM32F4)
	DDEFS += -DSHORT_BOARD_NAME=alphax-4chan
	# This board has trigger scope hardware!
    DDEFS += -DTRIGGER_SCOPE
    DDEFS += $(PRIMARY_COMMUNICATION_PORT_USART2)
else
$(error Unsupported PROJECT_CPU [$(PROJECT_CPU)])
endif
