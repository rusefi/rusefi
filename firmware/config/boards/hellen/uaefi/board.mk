# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp
# Set this if you want a default engine type other than normal
ifeq ($(VAR_DEF_ENGINE_TYPE),)
#  VAR_DEF_ENGINE_TYPE = -DDEFAULT_ENGINE_TYPE=engine_type_e::
endif

DDEFS += -DEFI_MAIN_RELAY_CONTROL=TRUE

#no mux on mm100

# Add them all together
DDEFS += -DFIRMWARE_ID=\"uaefi\" $(VAR_DEF_ENGINE_TYPE)
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE
# EGT chip
DDEFS += -DEFI_MAX_31855=TRUE

SHORT_BOARD_NAME=uaefi

DDEFS += -DHELLEN_BOARD_ID_DEBUG

DDEFS += -DHW_HELLEN_UAEFI=1

include $(BOARDS_DIR)/hellen/hellen-common100.mk
