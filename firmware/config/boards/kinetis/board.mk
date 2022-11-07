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

DDEFS += -DSHORT_BOARD_NAME=kin

# TODO: somehow this -DDUMMY is helping us to not mess up the parameters, why?!
# https://github.com/rusefi/rusefi/issues/684
DDEFS += -DDUMMY
DDEFS += -DCPU_MKE16F512VLH16
DDEFS += -DCPU_MKE16F512VLH16_cm4
DDEFS += -D__USE_CMSIS
DDEFS += -DEFI_ENABLE_ASSERTS=FALSE
DDEFS += -DCH_DBG_ENABLE_CHECKS=FALSE
DDEFS += -DCH_DBG_ENABLE_ASSERTS=FALSE
DDEFS += -DCH_DBG_ENABLE_STACK_CHECK=FALSE
DDEFS += -DCH_DBG_FILL_THREADS=FALSE
DDEFS += -DCH_DBG_THREADS_PROFILING=FALSE
