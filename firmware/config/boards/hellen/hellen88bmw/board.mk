# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARDS_DIR)/hellen/hellen88bmw/board_configuration.cpp
BOARDINC = $(BOARDS_DIR)/hellen/hellen88bmw

# Set this if you want a default engine type other than normal hellen88bmw
ifeq ($(VAR_DEF_ENGINE_TYPE),)
  VAR_DEF_ENGINE_TYPE = -DDEFAULT_ENGINE_TYPE=HELLEN_88_BMW
endif


DDEFS += -DEFI_MAIN_RELAY_CONTROL=TRUE

# Disable serial ports on this board as UART3 causes a DMA conflict with the SD card
DDEFS += -DTS_NO_PRIMARY=1

# Add them all together
DDEFS += -DFIRMWARE_ID=\"hellen88bmw\" $(VAR_DEF_ENGINE_TYPE)

DDEFS += -DSHORT_BOARD_NAME=hellen88bmw

include $(BOARDS_DIR)/hellen/hellen-common144.mk
