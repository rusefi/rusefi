# List of all the board related files.
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp
DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::G14

# This is an F429!
IS_STM32F429 = yes

# This board has no storage
DDEFS += -DEFI_FILE_LOGGING=FALSE
USE_FATFS = no

DDEFS += -DFIRMWARE_ID=\"m74_9\"
DDEFS += -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS
DDEFS += -DSHORT_BOARD_NAME=m74_9
