# List of all the board related files.
BOARDCPPSRC = $(PROJECT_DIR)/config/boards/f407-discovery/board_extra.cpp

# MCU defines
DDEFS += -DSTM32F407xx

# We are running on Frankenso hardware!
DDEFS += -DHW_FRANKENSO=1
DDEFS += -DDEFAULT_ENGINE_TYPE=DEFAULT_FRANKENSO

DDEFS += -DSTM32_I2C_USE_I2C3=TRUE

ifndef IS_RE_BOOTLOADER
DDEFS += -DHAL_USE_EEPROM=TRUE
endif