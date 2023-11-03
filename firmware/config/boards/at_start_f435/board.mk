# List of all the board related files.
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp \
  $(BOARD_DIR)/board_storage.cpp

DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::D13
DDEFS += -DLED_PIN_MODE=OM_INVERTED

# USB OTG1 connector:
DDEFS += -DEFI_USB_SERIAL_DM=Gpio::A11
DDEFS += -DEFI_USB_SERIAL_DP=Gpio::A12
DDEFS += -DEFI_USB_AF=10

IS_AT32F435 = yes

# Stock board.c from ChibiOS
BOARD_C = $(CHIBIOS)/os/hal/boards/AT_START_F435/board.c
# Stock board.h from ChibiOS
BOARDINC = $(CHIBIOS)/os/hal/boards/AT_START_F435/

# Configuration directorys
CONFDIR = $(PROJECT_DIR)/hw_layer/ports/at32/at32f4/cfg

# This board uses ChibiOS MFS driver on internal flash
DDEFS += -DEFI_STORAGE_INT_FLASH=FLASE -DHAL_USE_EFL=TRUE -DEFI_STORAGE_MFS=TRUE
# This board has chip with dual-bank flash, bank 2 can be flashed in background
DDEFS += -DEFI_FLASH_WRITE_THREAD=TRUE

DDEFS += -DFIRMWARE_ID=\"at_start_f435\"
DDEFS += -DDEFAULT_ENGINE_TYPE=engine_type_e::MINIMAL_PINS
DDEFS += -DSHORT_BOARD_NAME=at_start_f435 -DSTATIC_BOARD_ID=STATIC_BOARD_ID_AT_START_F435

DDEFS += -DEFI_BACKUP_SRAM=FALSE
