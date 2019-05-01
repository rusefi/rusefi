# List of all the board related files.
BOARDSRC = $(PROJECT_DIR)/config/boards/me7_pnp/board.c
BOARDSRC_CPP = $(PROJECT_DIR)/config/boards/me7_pnp/board_configuration.cpp

# Required include directories
BOARDINC = $(PROJECT_DIR)/config/boards/me7_pnp
# Override LD script
ifeq ($(USE_BOOTLOADER),yes)
  # include Prometheus bootloader code
  BOOTLOADERINC= $(PROJECT_DIR)/bootloader/subaru-ej20gn
endif

LDSCRIPT= $(PROJECT_DIR)/config/boards/me7_pnp/STM32F76xxI.ld


# Override DEFAULT_ENGINE_TYPE
DDEFS += -DDEFAULT_ENGINE_TYPE=VAG_18_TURBO -DSTM32F767xx
