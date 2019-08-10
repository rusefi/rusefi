# List of all the board related files.
BOARDSRC = $(CHIBIOS)/os/hal/boards/ST_NUCLEO144_F767ZI/board.c
BOARDSRC_CPP = $(PROJECT_DIR)/config/boards/microrusefi/board_configuration.cpp

# Required include directories
BOARDINC = $(PROJECT_DIR)/config/boards/nucleo_f767 $(PROJECT_DIR)/config/stm32f7ems

LDSCRIPT= $(PROJECT_DIR)/config/boards/nucleo_f767/STM32F76xxI.ld

# Override DEFAULT_ENGINE_TYPE
DDEFS += -DSTM32F767xx -DEFI_USE_OSC=TRUE -DFIRMWARE_ID=\"microRusEfi\"
