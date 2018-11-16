# List of all the board related files.
BOARDSRC = $(PROJECT_DIR)/config/boards/NUCLEO_F767/board.c
BOARDSRC_CPP = $(PROJECT_DIR)/config/boards/NUCLEO_F767/board_configuration.cpp

# Required include directories
BOARDINC = $(PROJECT_DIR)/config/boards/NUCLEO_F767 $(PROJECT_DIR)/config/stm32f7ems

LDSCRIPT= $(PROJECT_DIR)/config/boards/NUCLEO_F767/STM32F76xxI.ld

# Override DEFAULT_ENGINE_TYPE
DDEFS += -DDEFAULT_ENGINE_TYPE=CUSTOM_ENGINE -DSTM32F767xx
