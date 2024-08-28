

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp



DDEFS += -DEFI_MAIN_RELAY_CONTROL=TRUE
DDEFS += -DEFI_MC33816=TRUE
DDEFS += -DBOARD_TLE9104_COUNT=4





# we have hellen board id BOARD_ID_GDI4CHAN_A

DDEFS += -DHW_HELLEN_4K_GDI=1

include $(BOARDS_DIR)/hellen/hellen-common-mega144.mk

