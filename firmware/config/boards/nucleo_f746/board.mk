# List of all the board related files.
BOARDSRC = $(CHIBIOS)/os/hal/boards/ST_NUCLEO144_F746ZG/board.c
BOARDCPPSRC = $(PROJECT_DIR)/config/boards/nucleo_f767/board_configuration.cpp

# Required include directories
BOARDINC = $(PROJECT_DIR)/config/boards/nucleo_f746 $(PROJECT_DIR)/config/stm32f7ems
CONFDIR = config/stm32f7ems

LDSCRIPT = $(PROJECT_DIR)/config/boards/nucleo_f746/STM32F746xG.ld

# Override DEFAULT_ENGINE_TYPE
DDEFS += -DDEFAULT_ENGINE_TYPE=DEFAULT_FRANKENSO -DSTM32F746xx

# Shared variables
ALLCSRC   += $(BOARDSRC)
ALLCPPSRC += $(BOARDCPPSRC)
ALLINC    += $(BOARDINC)
