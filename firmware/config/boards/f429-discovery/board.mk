BOARD_DIR = $(PROJECT_DIR)/config/boards/$(PROJECT_BOARD)

HALCONFDIR = $(BOARD_DIR)

# List of all the board related files.
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp

# Required include directories
BOARDINC = $(BOARD_DIR)

# STM32F429 has FSMC with SDRAM support
IS_STM32F429 = yes

# avoid any engine setup
DDEFS += -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS

#LED
DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=GPIOG_14

DDEFS += -DSTM32_FSMC_USE_FSMC1=TRUE -DSTM32_SDRAM_USE_SDRAM2=TRUE

# Shared variables
ALLINC += $(BOARDINC)
