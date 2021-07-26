# List of all the board related files.
BOARDCPPSRC =  $(PROJECT_DIR)/config/boards/atlas/board_configuration.cpp

BOARDINC = $(PROJECT_DIR)/config/boards/atlas

PROJECT_CPU = ARCH_STM32H7

#DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=GPIOE_3
DDEFS += -DFIRMWARE_ID=\"atlas\" -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS

# This stuff doesn't work on H7 yet
ifneq ($(PROJECT_CPU),ARCH_STM32H7)
	DDEFS += -DSTM32_ADC_USE_ADC3=TRUE
	DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE
endif

# disable hardware serial ports on H7
DDEFS += -DTS_NO_PRIMARY -DTS_NO_SECONDARY

# We are running on Proteus hardware!
DDEFS += -DHW_ATLAS=1

# Shared variables
ALLCPPSRC += $(BOARDCPPSRC)
ALLINC    += $(BOARDINC)
