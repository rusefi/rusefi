# List of all the board related files.
BOARDCPPSRC = $(PROJECT_DIR)/config/boards/m74_9/board_configuration.cpp
BOARDINC = $(PROJECT_DIR)/config/boards/m74_9

DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::G14

# This is an F429!
IS_STM32F429 = yes

# This board has no storage
DDEFS += -DEFI_FILE_LOGGING=FALSE
USE_FATFS = no

DDEFS += -DFIRMWARE_ID=\"m74_9\"
DDEFS += -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS
