# List of all the board related files.
BOARDSRC = $(PROJECT_DIR)/config/boards/microrusefi/board.c
BOARDSRC_CPP = 

# Required include directories
BOARDINC = $(PROJECT_DIR)/config/stm32f7ems $(PROJECT_DIR)/config/boards/microrusefi

LDSCRIPT= $(PROJECT_DIR)/config/boards/microrusefi/STM32F76xxI.ld

# Override DEFAULT_ENGINE_TYPE
DDEFS += -DDEFAULT_ENGINE_TYPE=CUSTOM_ENGINE -DSTM32F767xx
