# List of all the board related files.
BOARDSRC = $(CHIBIOS)/os/hal/boards/ST_STM32F4_DISCOVERY/board.c \
    $(PROJECT_DIR)/config/boards/st_stm32f4/board_extra.c
BOARDSRC_CPP =

# Required include directories
BOARDINC = $(PROJECT_DIR)/config/boards/st_stm32f4

# MCU defines
DDEFS += -DSTM32F407xx
