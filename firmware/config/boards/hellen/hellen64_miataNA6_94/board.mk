# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp
# Set this if you want a default engine type other than normal hellen64_miataNA6_94
ifeq ($(VAR_DEF_ENGINE_TYPE),)
  VAR_DEF_ENGINE_TYPE = -DDEFAULT_ENGINE_TYPE=HELLEN_NA6
endif

DDEFS += -DEFI_MAIN_RELAY_CONTROL=TRUE

# Add them all together
DDEFS += -DFIRMWARE_ID=\"hellenNA6\" $(VAR_DEF_ENGINE_TYPE)
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE

# https://github.com/rusefi/hellenNA6_issues/issues/35 hack :(
DDEFS += -DDISABLE_PIN_STATE_VALIDATION=TRUE

SHORT_BOARD_NAME = hellenNA6

include $(BOARDS_DIR)/hellen/hellen-common176.mk
