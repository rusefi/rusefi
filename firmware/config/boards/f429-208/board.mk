BOARD_DIR = $(PROJECT_DIR)/config/boards/$(PROJECT_BOARD)

HALCONFDIR = $(BOARD_DIR)

# List of all the board related files.
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp

# Required include directories
BOARDINC = $(BOARD_DIR)

# STM32F429 has FSMC with SDRAM support
DDEFS += -DFIRMWARE_ID=\"stm32f429\"
IS_STM32F429 = yes
EFI_HAS_EXT_SDRAM = yes

#LED
DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::B11

DDEFS += -DSTM32_FSMC_USE_FSMC1=TRUE -DSTM32_SDRAM_USE_SDRAM2=TRUE

DDEFS += -DHAL_USE_SDRAM=TRUE
DDEFS += -DHAL_USE_FSMC=TRUE

# see signature_haba208.h
DDEFS += -DSHORT_BOARD_NAME=haba208

# Shared variables
ALLINC += $(BOARDINC)
