# List of all the board related files.
BOARDCPPSRC = $(BOARD_DIR)/board_extra.cpp

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

# See also ts_show_critical_led
DDEFS += -DFLEXIBLE_CRITICAL_LED=1

# User can configure LIN/K-line interface
DDEFS += -DEFI_KLINE=TRUE
DDEFS += -DKLINE_SERIAL_DEVICE_RX=Gpio::C11 -DKLINE_SERIAL_DEVICE_TX=Gpio::C10
DDEFS += -DKLINE_SERIAL_DEVICE=SD3

DDEFS += -DBOARD_TLE9104_COUNT=2

# We are running on Frankenso hardware!
DDEFS += -DHW_FRANKENSO=1
DDEFS += $(DEFAULT_ENGINE_TYPE)

DDEFS += -DSTM32_ADC_USE_ADC3=TRUE
# todo: make knock pin software-selectable?
# todo: DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE

DDEFS += -DEFI_SENT_SUPPORT=TRUE

# TS_SECONDARY_UxART_PORT
DDEFS += -DSTM32_SERIAL_USE_USART3=TRUE
DDEFS += -DTS_SECONDARY_UxART_PORT=SD3

DDEFS += -DSTM32_I2C_USE_I2C3=TRUE

DDEFS += -DEFI_WS2812=TRUE

ifndef IS_RE_BOOTLOADER
DDEFS += -DHAL_USE_EEPROM=TRUE
endif