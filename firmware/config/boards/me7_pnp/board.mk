# List of all the board related files.
BOARDCPPSRC = $(PROJECT_DIR)/config/boards/me7_pnp/board_configuration.cpp

# Override DEFAULT_ENGINE_TYPE
DDEFS += -DDEFAULT_ENGINE_TYPE=VAG_18_TURBO -DSTM32F767xx

# Shared variables
ALLCSRC   += $(BOARDSRC)
ALLCPPSRC += $(BOARDCPPSRC)
ALLINC    += $(BOARDINC)
