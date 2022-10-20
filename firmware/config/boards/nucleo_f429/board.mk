# List of all the board related files.
BOARDCPPSRC = $(PROJECT_DIR)/config/boards/nucleo_f429/board_configuration.cpp
BOARDINC = $(PROJECT_DIR)/config/boards/nucleo_f429

DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::B14

# Enable ethernet
LWIP = yes
DDEFS += -DEFI_ETHERNET=TRUE

# This is an F429!
IS_STM32F429 = yes

DDEFS += -DFIRMWARE_ID=\"nucleo_f429\"
DDEFS += -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS
