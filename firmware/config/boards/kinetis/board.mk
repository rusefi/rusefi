# List of all the board related files.

BOARD_DIR = $(PROJECT_DIR)/config/boards/$(PROJECT_BOARD)

BOARDSRC = $(BOARD_DIR)/board.c
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp

# Required include directories
BOARDINC  = $(BOARD_DIR)
BOARDINC += $(BOARD_DIR)/config/controllers/algo

BOARDCPPSRC += $(CONTROLLERS_ALGO_AUTO_GENERATED_ENUMS)

# Define linker script file here
LDSCRIPT = $(STARTUPLD_KINETIS)/MKE1xF512.ld

CONFDIR = $(BOARD_DIR)

# Shared variables
ALLCSRC   += $(BOARDSRC)
ALLCPPSRC += $(BOARDCPPSRC)
ALLINC    += $(BOARDINC)
