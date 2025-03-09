# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp
# Set this if you want a default engine type other than normal
ifeq ($(VAR_DEF_ENGINE_TYPE),)
#  VAR_DEF_ENGINE_TYPE = -DDEFAULT_ENGINE_TYPE=engine_type_e::HELLEN_154_HYUNDAI_COUPE_BK2
endif



#no mux on mm100

# Add them all together
DDEFS += -DFIRMWARE_ID=\"hellen-112-17\" $(VAR_DEF_ENGINE_TYPE)
#DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE

DDEFS += -DHELLEN_BOARD_ID_DEBUG

ONBOARD_MEMS_TYPE=LIS2DH12

include $(BOARDS_DIR)/hellen/hellen-common100.mk
