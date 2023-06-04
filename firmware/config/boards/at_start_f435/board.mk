# List of all the board related files.
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp
DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::D13

DDEFS += -DFIRMWARE_ID=\"at_start_f435\"
DDEFS += -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS
DDEFS += -DSHORT_BOARD_NAME=at_start_f435
