# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp

# Set this if you want a default engine type other than normal Hellen72
ifeq ($(VAR_DEF_ENGINE_TYPE),)
  VAR_DEF_ENGINE_TYPE = -DDEFAULT_ENGINE_TYPE=engine_type_e::MAZDA_MIATA_NB2
endif




# Add them all together
DDEFS += -DFIRMWARE_ID=\"hellen72\" $(VAR_DEF_ENGINE_TYPE)
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE
DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_HELLEN_72

# this board uses original MCU module before mega-module generation
LED_CRITICAL_ERROR_BRAIN_PIN = -DLED_CRITICAL_ERROR_BRAIN_PIN=H176_ORIGINAL_MCU_LED1_RED
include $(BOARDS_DIR)/hellen/hellen-common176.mk
DDEFS += -DHW_HELLEN_NB2=1
