# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARDS_DIR)/hellen/hellen-gm-e67/board_configuration.cpp
BOARDINC = $(BOARDS_DIR)/hellen/hellen-gm-e67


DDEFS += -DEFI_MAIN_RELAY_CONTROL=TRUE

# Add them all together
DDEFS += -DFIRMWARE_ID=\"hellenE67\" $(VAR_DEF_ENGINE_TYPE)
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE

# No hardware serial TS
DDEFS += -DTS_NO_PRIMARY=1

include $(BOARDS_DIR)/hellen/hellen-common144.mk


DDEFS += -DSHORT_BOARD_NAME=hellen-gm-e67
