# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp


DDEFS += -DKNOCK_SPECTROGRAM=TRUE

# pretty temporary?
DDEFS += -DDISABLE_PIN_STATE_VALIDATION=TRUE

# temporary or not?
DDEFS += -DETB_INTERMITTENT_LIMIT=60001

# quick board start-up with less fancy bootloader
DDEFS += -DBOOT_BACKDOOR_ENTRY_TIMEOUT_MS=0

DDEFS += -DEFI_LOGIC_ANALYZER=FALSE
DDEFS += -DEFI_MALFUNCTION_INDICATOR=FALSE

include $(BOARDS_DIR)/hellen/hellen-common-mega144.mk

# This board has trigger scope hardware!
DDEFS += -DTRIGGER_SCOPE

ifeq ($(PROJECT_CPU),ARCH_STM32F7)
  # need boot times of under 350ms for car that expects fast CAN https://github.com/rusefi/alphax-4chan/issues/184
  DDEFS += -DHW_HELLEN_SKIP_BOARD_TYPE=TRUE
	DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_ALPHAX_4CHAN_F7
    # TODO do we only support serial on F7 but not UART?
    DDEFS += -DEFI_CONSOLE_TX_BRAIN_PIN=Gpio::D6 -DEFI_CONSOLE_RX_BRAIN_PIN=Gpio::D5
    DDEFS += -DTS_PRIMARY_UxART_PORT=SD2 -DEFI_TS_PRIMARY_IS_SERIAL=TRUE -DSTM32_SERIAL_USE_USART2=TRUE -DSTM32_UART_USE_USART2=FALSE
else ifeq ($(PROJECT_CPU),ARCH_STM32F4)
	DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_ALPHAX_4CHAN
	DDEFS += $(PRIMARY_COMMUNICATION_PORT_USART2)
else
$(error Unsupported PROJECT_CPU [$(PROJECT_CPU)])
endif

DDEFS += -DHW_HELLEN_4CHAN=1
