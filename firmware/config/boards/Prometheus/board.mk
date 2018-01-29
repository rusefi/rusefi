# List of all the board related files.
BOARDSRC = $(PROJECT_DIR)/config/boards/Prometheus/board.c
BOARDSRC_CPP = $(PROJECT_DIR)/config/boards/Prometheus/board_configuration.cpp

# Required include directories
BOARDINC = $(PROJECT_DIR)/config/boards/Prometheus

# Override LD script
ifeq ($(USE_BOOTLOADER),yes)
  # include Prometheus bootloader code
  BOOTLOADERINC= $(PROJECT_DIR)/bootloader/Prometheus
endif

LDSCRIPT= $(PROJECT_DIR)/config/boards/Prometheus/STM32F469xI.ld

# Override DEFAULT_ENGINE_TYPE
DDEFS += -DDEFAULT_ENGINE_TYPE=PROMETHEUS_DEFAULTS -DSTM32F469xx
