

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp



DDEFS += -DEFI_MAIN_RELAY_CONTROL=TRUE
DDEFS += -DBOARD_MC33810_COUNT=1








include $(BOARDS_DIR)/hellen/hellen-common-mega144.mk

