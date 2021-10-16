# List of all the board related files.
BOARDCPPSRC = $(PROJECT_DIR)/config/boards/nucleo_f767/board_configuration.cpp

# Override DEFAULT_ENGINE_TYPE
DDEFS += -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS -DSTM32F767xx

# Shared variables
ALLCPPSRC += $(BOARDCPPSRC)
ALLINC    += $(BOARDINC)
