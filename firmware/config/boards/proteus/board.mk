# List of all the board related files.
BOARDCPPSRC =  $(BOARD_DIR)/board_configuration.cpp

ifeq ($(PROJECT_CPU),ARCH_STM32F4)
  IS_STM32F429 = yes
endif

# see also openblt/board.mk STATUS_LED
DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::E3
DDEFS += -DFIRMWARE_ID=\"proteus\"
DDEFS += $(VAR_DEF_ENGINE_TYPE)


DDEFS += -DEFI_HD_ACR=TRUE
DDEFS += -DEFI_MAX_31855=TRUE
DDEFS += -DSTM32_SPI_USE_SPI5=TRUE

DDEFS += -DBOOT_COM_RS232_RX_MAX_DATA=120
DDEFS += -DOPEN_BLT_TEST_COMMAND=1

# let's start asap
DDEFS += -DBOOT_BACKDOOR_ENTRY_TIMEOUT_MS=0

# Any Proteus-based adapter boards with discrete-VR decoder are controlled via a 5v ignition output
DDEFS += -DVR_SUPPLY_VOLTAGE=5

# This stuff doesn't work on H7 yet
ifneq ($(PROJECT_CPU),ARCH_STM32H7)
	DDEFS += -DSTM32_ADC_USE_ADC3=TRUE
	DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE
endif

# serial ports only on F4
ifeq ($(PROJECT_CPU),ARCH_STM32F4)
	# Hardware serial port on UART 2 -> PD5/PD6
	DDEFS += -DSTM32_UART_USE_USART2=TRUE
	DDEFS += -DTS_PRIMARY_UxART_PORT=UARTD2
	DDEFS += -DEFI_CONSOLE_TX_BRAIN_PIN=Gpio::D5 -DEFI_CONSOLE_RX_BRAIN_PIN=Gpio::D6
else
  # F7, H7
  DDEFS += -DLUA_canFrameCount=256
endif

# CAND1
DDEFS += -DBOOT_COM_CAN_CHANNEL_INDEX=0
DDEFS += -DOPENBLT_CAN_RX_PORT=GPIOD
DDEFS += -DOPENBLT_CAN_RX_PIN=0
DDEFS += -DOPENBLT_CAN_TX_PORT=GPIOD
DDEFS += -DOPENBLT_CAN_TX_PIN=1

# We are running on Proteus hardware!
DDEFS += -DHW_PROTEUS=1

ifeq ($(PROJECT_CPU),ARCH_STM32F7)
  # todo: KNOCK_SPECTROGRAM to use big_buffer
	DDEFS += -DKNOCK_SPECTROGRAM=TRUE
	DDEFS += -DLUA_RX_MAX_FILTER_COUNT=96
	DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_PROTEUS_F7

  # technical debt: UI uses separate ts_show_tcu flag
  DDEFS += -DEFI_TCU=TRUE

	ifeq ($(EFI_LUA_LOOKUP),)
    EFI_LUA_LOOKUP = TRUE
  endif
  DDEFS += -DEFI_LUA_LOOKUP=$(EFI_LUA_LOOKUP)

	# note #define EFI_EMBED_INI_MSD FALSE in F7 features
	ifeq ($(DEBUG_LEVEL_OPT),)
		DEBUG_LEVEL_OPT = -Os -ggdb -g
	endif
else ifeq ($(PROJECT_CPU),ARCH_STM32F4)
	DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_PROTEUS_F4
	# todo: https://github.com/rusefi/rusefi/pull/7505
	# you would think that removal of below like would reduce flash usage but somehow it increased it somehow?!
	DDEFS += -DKNOCK_SPECTROGRAM=TRUE
else ifeq ($(PROJECT_CPU),ARCH_STM32H7)
	DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_PROTEUS_H7
else
$(error Unsupported PROJECT_CPU [$(PROJECT_CPU)])
endif
