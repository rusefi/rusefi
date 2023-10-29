# List of all the board related files.
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp
DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::D13
DDEFS += -DLED_PIN_MODE=OM_INVERTED

IS_AT32F435 = yes

# Stock board.c from ChibiOS
BOARD_C = $(CHIBIOS)/os/hal/boards/AT_START_F435/board.c
# Stock board.h from ChibiOS
BOARDINC = $(CHIBIOS)/os/hal/boards/AT_START_F435/

# Configuration directorys
CONFDIR = $(PROJECT_DIR)/hw_layer/ports/at32/at32f4/cfg

DDEFS += -DFIRMWARE_ID=\"at_start_f435\"
DDEFS += -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS
DDEFS += -DSHORT_BOARD_NAME=at_start_f435 -DSTATIC_BOARD_ID=STATIC_BOARD_ID_AT_START_F435

DDEFS += -DEFI_BACKUP_SRAM=FALSE
