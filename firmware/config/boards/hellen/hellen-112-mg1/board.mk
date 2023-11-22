# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp
# Set this if you want a default engine type other than normal
ifeq ($(VAR_DEF_ENGINE_TYPE),)
#  VAR_DEF_ENGINE_TYPE = -DDEFAULT_ENGINE_TYPE=engine_type_e::HELLEN_154_HYUNDAI_COUPE_BK2
endif

DDEFS += -DEFI_MAIN_RELAY_CONTROL=TRUE

#no mux on mm100

# Add them all together
DDEFS += -DFIRMWARE_ID=\"hellen-112-mg1\" $(VAR_DEF_ENGINE_TYPE)
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE
DDEFS += -DBOARD_MC33810_COUNT=1

SHORT_BOARD_NAME=hellen-112-mg1

# DDEFS += -DHELLEN_BOARD_ID_DEBUG


ifeq ($(PROJECT_CPU),ARCH_STM32F7)
    USE_OPT += -Wl,--defsym=FLASH_SIZE=768k
endif

include $(BOARDS_DIR)/hellen/hellen-common100.mk
