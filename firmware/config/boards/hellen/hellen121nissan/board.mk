# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp
# Set this if you want a default engine type other than normal hellen121nissan
ifeq ($(VAR_DEF_ENGINE_TYPE),)
  VAR_DEF_ENGINE_TYPE = -DDEFAULT_ENGINE_TYPE=HELLEN_121_NISSAN_6_CYL
endif

DDEFS += -DEFI_MAIN_RELAY_CONTROL=TRUE

# Add them all together
DDEFS += -DFIRMWARE_ID=\"hellen121nissan\" $(VAR_DEF_ENGINE_TYPE)
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE

# bad temporary solution of hardware issue https://github.com/rusefi/rusefi/issues/4821
DDEFS += -DMAX_TPS_PPS_DISCREPANCY=10.0f

SHORT_BOARD_NAME = hellen121nissan

include $(BOARDS_DIR)/hellen/hellen-common144.mk
