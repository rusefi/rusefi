# List of all the board related files.
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp \
  $(BOARD_DIR)/../at_start_f435/board_storage.cpp

DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::Unassigned

IS_AT32F435 = yes

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
USE_FATFS = no

# Configuration directorys
CONFDIR = $(PROJECT_DIR)/hw_layer/ports/at32/at32f4/cfg

# This board uses ChibiOS MFS driver on internal flash
DDEFS += -DHAL_USE_EFL=TRUE
include $(PROJECT_DIR)/hw_layer/ports/stm32/use_higher_level_flash_api.mk
# This board has chip with dual-bank flash, bank 2 can be flashed in background
DDEFS += -DEFI_FLASH_WRITE_THREAD=TRUE

DDEFS += -DFIRMWARE_ID=\"m74_9\"
DDEFS += -DDEFAULT_ENGINE_TYPE=engine_type_e::MINIMAL_PINS
DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_M74_9

DDEFS += -DEFI_BACKUP_SRAM=FALSE
