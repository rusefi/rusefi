# List of all the board related files.
BOARDCPPSRC =  $(PROJECT_DIR)/config/boards/core8/board_configuration.cpp

BOARDINC = $(PROJECT_DIR)/config/boards/core8

# Override DEFAULT_ENGINE_TYPE
DDEFS += -DSHORT_BOARD_NAME=core8
DDEFS += -DFIRMWARE_ID=\"core8\"
DDEFS += -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS
