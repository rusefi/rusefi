# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp
# Set this if you want a default engine type other than normal
ifeq ($(VAR_DEF_ENGINE_TYPE),)
#  VAR_DEF_ENGINE_TYPE = -DDEFAULT_ENGINE_TYPE=engine_type_e::HELLEN_154_HYUNDAI_COUPE_BK2
endif

DDEFS += -DEFI_MAIN_RELAY_CONTROL=TRUE

DDEFS += -DEFI_KLINE=TRUE
DDEFS += -DSTM32_SERIAL_USE_USART2=TRUE

DDEFS += -DADC_MUX_PIN=Gpio::F2

DDEFS += -DRAM_UNUSED_SIZE=6000

# Add them all together
DDEFS += -DFIRMWARE_ID=\"hellen-honda-k\" $(VAR_DEF_ENGINE_TYPE)
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE
DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_HELLEN_HONDA_K

SHORT_BOARD_NAME=hellen-honda-k


include $(BOARDS_DIR)/hellen/hellen-common144.mk
DDEFS += -DHW_HELLEN_HONDA=1
DDEFS += -DHELLEN_BOARD_ID_DEBUG
