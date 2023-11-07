# List of all the board related files.
BOARDCPPSRC =  $(BOARD_DIR)/board_configuration.cpp

# Override DEFAULT_ENGINE_TYPE
SHORT_BOARD_NAME = core8
DDEFS += -DFIRMWARE_ID=\"core8\"
DDEFS += -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS
