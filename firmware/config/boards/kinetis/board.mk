# List of all the board related files.

BOARD_DIR = $(PROJECT_DIR)/config/boards/$(PROJECT_BOARD)

BOARDSRC = $(BOARD_DIR)/board.c
BOARDSRC_CPP = $(BOARD_DIR)/board_configuration.cpp

# Required include directories
BOARDINC = $(BOARD_DIR)
BOARDINC += $(BOARD_DIR)/config/controllers/algo

BOARDSRC_CPP += $(CONTROLLERS_ALGO_AUTO_GENERATED_ENUMS)

# Define linker script file here
LDSCRIPT= $(STARTUPLD)/MKE1xF512.ld

PLATFORMSRC += $(PLATFORMSRC_CONTRIB)
PLATFORMINC += $(PLATFORMINC_CONTRIB)

# Using external ChibiOS-Contrib code for Comparator HAL
HALSRC += $(CHIBIOS_CONTRIB)/os/hal/src/hal_comp.c

HW_LAYER_EMS_CPP += $(PROJECT_DIR)/hw_layer/trigger_input_comp.cpp
