# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp
# Set this if you want a default engine type other than normal hellen64_miataNA6_94
ifeq ($(VAR_DEF_ENGINE_TYPE),)
  VAR_DEF_ENGINE_TYPE = -DDEFAULT_ENGINE_TYPE=engine_type_e::MAZDA_MIATA_NA6
endif



# Add them all together
DDEFS += -DFIRMWARE_ID=\"hellenNA6\" $(VAR_DEF_ENGINE_TYPE)
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE

# https://github.com/rusefi/hellenNA6_issues/issues/35 hack :(
DDEFS += -DDISABLE_PIN_STATE_VALIDATION=TRUE
DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_HELLEN_NA6

DDEFS += -DHW_HELLEN_MIATA_NA6=1

# this board uses original MCU module before mega-module generation
LED_CRITICAL_ERROR_BRAIN_PIN = -DLED_CRITICAL_ERROR_BRAIN_PIN=H176_ORIGINAL_MCU_LED1_RED
include $(BOARDS_DIR)/hellen/hellen-common176.mk
