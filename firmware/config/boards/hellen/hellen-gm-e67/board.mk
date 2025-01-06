# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp

# Add them all together
DDEFS += -DFIRMWARE_ID=\"hellenE67\" $(VAR_DEF_ENGINE_TYPE)
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE

include $(BOARDS_DIR)/hellen/hellen-common144.mk

DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_HELLEN_GM_E67

