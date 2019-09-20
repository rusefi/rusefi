# List of all the board related files.
BOARDSRC = $(CHIBIOS)/os/hal/boards/ST_NUCLEO144_F767ZI/board.c
BOARDSRC_CPP = $(PROJECT_DIR)/config/boards/microrusefi/board_configuration.cpp

# Required include directories
BOARDINC = $(PROJECT_DIR)/config/boards/nucleo_f767 $(PROJECT_DIR)/config/stm32f7ems

LDSCRIPT= $(PROJECT_DIR)/config/boards/nucleo_f767/STM32F76xxI.ld

# Set this if you want a default engine type other than normal MRE
ifeq ($(DEFAULT_ENGINE_TYPE),)
  DEFAULT_ENGINE_TYPE = -DDEFAULT_ENGINE_TYPE=MICRO_RUS_EFI
endif

# Override DEFAULT_ENGINE_TYPE
DDEFS += -DSTM32F767xx -DEFI_USE_OSC=TRUE -DEFI_FATAL_ERROR_PIN=GPIOE_3 -DFIRMWARE_ID=\"microRusEfi\" $(DEFAULT_ENGINE_TYPE)
