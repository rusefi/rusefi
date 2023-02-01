# List of all the board related files.
BOARDCPPSRC =  $(BOARD_DIR)/board_configuration.cpp

# Override DEFAULT_ENGINE_TYPE
DDEFS += -DSHORT_BOARD_NAME=48way
DDEFS += -DFIRMWARE_ID=\"48way\"
DDEFS += -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS
