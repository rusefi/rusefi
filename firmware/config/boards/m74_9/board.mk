# List of all the board related files.
ifeq ($(IS_RE_BOOTLOADER),yes)
BOARDCPPSRC = $(BOARD_DIR)/board_bootloader.cpp
else
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp \
  $(BOARD_DIR)/../at_start_f435/board_storage.cpp
endif

DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::Unassigned

IS_AT32F435 = yes

# OpenBLT replacement-loader profile: CAN1 on PG0/PG1, no USB transport.
ifeq ($(USE_OPENBLT),yes)
DDEFS += -DEFI_USE_OPENBLT=TRUE
DDEFS += -DBOOT_COM_RS232_ENABLE=0
DDEFS += -DBOOT_COM_CAN_CHANNEL_INDEX=0
DDEFS += -DOPENBLT_CAN_RX_PORT=GPIOG -DOPENBLT_CAN_RX_PIN=0
DDEFS += -DOPENBLT_CAN_TX_PORT=GPIOG -DOPENBLT_CAN_TX_PIN=1
DDEFS += -DBOOT_BACKDOOR_ENTRY_TIMEOUT_MS=1000
endif

# board.c from this directory
BOARD_C = $(BOARD_DIR)/board.c
# board.h from this directory
BOARDINC = $(BOARD_DIR)

#This board has no USB wired out
DDEFS += -DSTM32_USB_USE_OTG1=FALSE
DDEFS += -DSTM32_USB_USE_OTG2=FALSE

DDEFS += -DBOARD_L9779_COUNT=1

# This board has no storage
DDEFS += -DEFI_FILE_LOGGING=FALSE
DDEFS += -DEFI_STORAGE_SD=FALSE
USE_FATFS = no

# Configuration directorys
CONFDIR = $(PROJECT_DIR)/hw_layer/ports/at32/at32f4/cfg

# This board uses ChibiOS MFS driver on internal flash
DDEFS += -DHAL_USE_EFL=TRUE
DDEFS += -DEFI_STORAGE_INT_FLASH=FALSE
include $(PROJECT_DIR)/hw_layer/ports/stm32/use_higher_level_flash_api.mk

DDEFS += -DFIRMWARE_ID=\"m74_9\"
DDEFS += -DDEFAULT_ENGINE_TYPE=engine_type_e::MINIMAL_PINS
DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_M74_9

DDEFS += -DEFI_BACKUP_SRAM=FALSE
DDEFS += -DEFI_UDS=TRUE
