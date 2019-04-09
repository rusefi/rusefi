# List of all the board related files.
BOARDSRC = $(PROJECT_DIR)/config/boards/st_stm32f4/board.c
BOARDSRC_CPP =

# Required include directories
BOARDINC = $(PROJECT_DIR)/config/boards/st_stm32f4

# MCU defines
DDEFS += -DSTM32F407xx
