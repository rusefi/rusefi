# List of all the board related files.
BOARDSRC = $(PROJECT_DIR)/config/boards/ST_STM32F4/board.c
BOARDSRC_CPP =

# Required include directories
BOARDINC = $(PROJECT_DIR)/config/boards/ST_STM32F4

# MCU defines
DDEFS += -DSTM32F407xx
