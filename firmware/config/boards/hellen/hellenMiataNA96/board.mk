# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp
ifeq ($(VAR_DEF_ENGINE_TYPE),)
  VAR_DEF_ENGINE_TYPE = -DDEFAULT_ENGINE_TYPE=engine_type_e::MAZDA_MIATA_NA96
endif



# Add them all together
DDEFS += -DFIRMWARE_ID=\"hellenNB1\" $(VAR_DEF_ENGINE_TYPE)
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE
include $(BOARDS_DIR)/hellen/hellen-common144.mk
DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_HELLEN_NA8_96

# Enable serial pins on expansion header
DDEFS += $(PRIMARY_COMMUNICATION_PORT_USART2)

