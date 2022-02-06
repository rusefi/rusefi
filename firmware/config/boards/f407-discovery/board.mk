# List of all the board related files.
BOARDSRC = $(PROJECT_DIR)/config/boards/f407-discovery/board_extra.c

# MCU defines
DDEFS += -DSTM32F407xx

# We are running on Frankenso hardware!
DDEFS += -DHW_FRANKENSO=1
DDEFS += -DSTM32_I2C_USE_I2C3=TRUE

ifndef IS_RE_BOOTLOADER
DDEFS += -DHAL_USE_EEPROM=TRUE
endif