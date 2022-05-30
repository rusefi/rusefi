# List of all the board related files.
BOARDCPPSRC =  $(PROJECT_DIR)/config/boards/48way/board_configuration.cpp

BOARDINC = $(PROJECT_DIR)/config/boards/48way

# Override DEFAULT_ENGINE_TYPE
DDEFS += -DSHORT_BOARD_NAME=48way
DDEFS += -DFIRMWARE_ID=\"48way\"
DDEFS += -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS
