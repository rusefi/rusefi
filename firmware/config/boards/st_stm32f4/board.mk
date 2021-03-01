# List of all the board related files.
BOARDSRC = $(PROJECT_DIR)/config/boards/st_stm32f4/board_extra.c

# MCU defines
DDEFS += -DSTM32F407xx

# We are running on Frankenso hardware!
DDEFS += -DHW_FRANKENSO=1

# Shared variables
ALLCSRC   += $(BOARDSRC)
ALLCPPSRC += $(BOARDCPPSRC)
ALLINC    += $(BOARDINC)
