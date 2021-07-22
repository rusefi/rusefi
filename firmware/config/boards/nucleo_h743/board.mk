# List of all the board related files.

# Shared variables
ALLCPPSRC += $(PROJECT_DIR)/config/boards/nucleo_h743/board_configuration.cpp

# Shared variables
ALLCSRC   += $(BOARDSRC)
ALLCPPSRC += $(BOARDCPPSRC)
ALLINC    += $(BOARDINC)

DDEFS += -DEFI_ICU_INPUTS=FALSE -DHAL_TRIGGER_USE_PAL=TRUE -DEFI_LOGIC_ANALYZER=FALSE
