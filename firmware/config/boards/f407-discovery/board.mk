# List of all the board related files.
BOARDCPPSRC = $(PROJECT_DIR)/config/boards/f407-discovery/board_extra.cpp

# MCU defines
DDEFS += -DSTM32F407xx

ifeq ($(VAR_DEF_ENGINE_TYPE),)
  VAR_DEF_ENGINE_TYPE = -DDEFAULT_ENGINE_TYPE=DEFAULT_FRANKENSO
endif

# here we use different names for env variable and macro name in order to reduce confusion. overall this is about Frankenso builds defining FIRMWARE_ID
ifeq ($(FW_ID_ENV),)
  DDEFS += -DFIRMWARE_ID=\"stm32f407vg\"
else
  DDEFS += -D$(FW_ID_ENV)
endif

# We are running on Frankenso hardware!
DDEFS += -DHW_FRANKENSO=1
DDEFS += $(DEFAULT_ENGINE_TYPE)

DDEFS += -DSTM32_I2C_USE_I2C3=TRUE

ifndef IS_RE_BOOTLOADER
DDEFS += -DHAL_USE_EEPROM=TRUE
endif