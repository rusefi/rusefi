# List of all the board related files.
BOARDCPPSRC =  $(BOARD_DIR)/../board_configuration.cpp

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

# CAND1
DDEFS += -DBOOT_COM_CAN_CHANNEL_INDEX=0
DDEFS += -DOPENBLT_CAN_RX_PORT=GPIOD
DDEFS += -DOPENBLT_CAN_RX_PIN=0
DDEFS += -DOPENBLT_CAN_TX_PORT=GPIOD
DDEFS += -DOPENBLT_CAN_TX_PIN=1

# We are running on Proteus hardware!
DDEFS += -DHW_PROTEUS=1

