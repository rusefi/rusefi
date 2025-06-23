# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp
# Set this if you want a default engine type other than normal hellen121vag
ifeq ($(VAR_DEF_ENGINE_TYPE),)
  VAR_DEF_ENGINE_TYPE = -DDEFAULT_ENGINE_TYPE=engine_type_e::HELLEN_121_VAG_4_CYL
endif

# Add them all together
DDEFS += -DFIRMWARE_ID=\"hellen121vag\" $(VAR_DEF_ENGINE_TYPE)
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE

# Use less Flash memory for this config
ifeq ($(EFI_LUA_LOOKUP),)
  EFI_LUA_LOOKUP = FALSE
endif
DDEFS += -DEFI_LUA_LOOKUP=$(EFI_LUA_LOOKUP)
DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_HELLEN_121VAG

# this board uses original MCU module before mega-module generation
LED_CRITICAL_ERROR_BRAIN_PIN = -DLED_CRITICAL_ERROR_BRAIN_PIN=H176_ORIGINAL_MCU_LED1_RED
include $(BOARDS_DIR)/hellen/hellen-common176.mk

DDEFS += -DHW_HELLEN_121_VAG=1


# we do not have much Lua RAM, let's drop some fancy functions
DDEFS += -DWITH_LUA_CONSUMPTION=FALSE
DDEFS += -DWITH_LUA_PID=FALSE
DDEFS += -DWITH_LUA_STOP_ENGINE=FALSE
