# List of all the board related files.

BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp

DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::B14

# Enable ethernet
LWIP = yes
DDEFS += -DEFI_ETHERNET=TRUE

DDEFS += -DFIRMWARE_ID=\"nucleo_h743\"
DDEFS += -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS
