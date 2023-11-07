# List of all the board related files.
BOARDCPPSRC =  $(BOARD_DIR)/board_configuration.cpp

#DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::E3
DDEFS += -DFIRMWARE_ID=\"atlas\"

# This stuff doesn't work on H7 yet
# DDEFS += -DSTM32_ADC_USE_ADC3=TRUE
# DEFS += -DEFI_SOFTWARE_KNOCK=TRUE

# We are running on Atlas hardware!
DDEFS += -DHW_ATLAS=1

# Atlas needs networking library
LWIP = yes
DDEFS += -DEFI_ETHERNET=TRUE

SHORT_BOARD_NAME = atlas
