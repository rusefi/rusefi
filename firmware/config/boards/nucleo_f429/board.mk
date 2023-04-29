# List of all the board related files.
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp
DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::B14

# Enable ethernet
LWIP = yes
DDEFS += -DEFI_ETHERNET=TRUE

# This is an F429!
IS_STM32F429 = yes

DDEFS += -DFIRMWARE_ID=\"nucleo_f429\"
DDEFS += -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS
DDEFS += -DSHORT_BOARD_NAME=nucleo_f429
