# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp
# Set this if you want a default engine type other than normal hellen88bmw
ifeq ($(VAR_DEF_ENGINE_TYPE),)
  VAR_DEF_ENGINE_TYPE = -DDEFAULT_ENGINE_TYPE=engine_type_e::MINIMAL_PINS
endif




# Add them all together
DDEFS += -DFIRMWARE_ID=\"hellen88bmw\" $(VAR_DEF_ENGINE_TYPE)
DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_HELLEN_88_BMW

include $(BOARDS_DIR)/hellen/hellen-common144.mk
