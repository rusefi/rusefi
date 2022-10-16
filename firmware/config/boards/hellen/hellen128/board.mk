# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARDS_DIR)/hellen/hellen128/board_configuration.cpp

BOARDINC = $(BOARDS_DIR)/hellen/hellen128

#BRDFLAGS     = -DSTM32F427xx

# Set this if you want a default engine type other than normal hellen128
ifeq ($(VAR_DEF_ENGINE_TYPE),)
  VAR_DEF_ENGINE_TYPE = -DDEFAULT_ENGINE_TYPE=HELLEN_128_MERCEDES_4_CYL
endif


DDEFS += -DEFI_MAIN_RELAY_CONTROL=TRUE

# Disable serial ports on this board as UART3 causes a DMA conflict with the SD card
DDEFS += -DTS_NO_PRIMARY=1

# Add them all together
DDEFS += -DFIRMWARE_ID=\"hellen128\" $(VAR_DEF_ENGINE_TYPE)
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE

DDEFS += -DSHORT_BOARD_NAME=hellen128

#Add TS_SECONDAY port
# PE0 and PE1 - UART8
DDEFS += -DSTM32_SERIAL_USE_UART8=TRUE
DDEFS += -DTS_SECONDARY_PORT=SD8
DDEFS += -DSTM32F427xx


include $(BOARDS_DIR)/hellen/hellen-common176.mk

