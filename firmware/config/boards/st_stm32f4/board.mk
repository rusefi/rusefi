# List of all the board related files.
BOARDSRC = $(CHIBIOS)/os/hal/boards/ST_STM32F4_DISCOVERY/board.c \
    $(PROJECT_DIR)/config/boards/st_stm32f4/board_extra.c
BOARDCPPSRC =

# Required include directories
BOARDINC = $(PROJECT_DIR)/config/boards/st_stm32f4

# MCU defines
DDEFS += -DSTM32F407xx

# We are running on Frankenso hardware!
DDEFS += -DHW_FRANKENSO=1

# Frankenso hardware has default self stim pins set
DDEFS += -DEFI_SIMULATOR_OUTPUT_1=GPIOD_1
DDEFS += -DEFI_SIMULATOR_OUTPUT_2=GPIOD_2

# Shared variables
ALLCSRC   += $(BOARDSRC)
ALLCPPSRC += $(BOARDCPPSRC)
ALLINC    += $(BOARDINC)
