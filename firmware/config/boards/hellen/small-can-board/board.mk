BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp

DDEFS += -DFIRMWARE_ID=\"small-can-board\"
DDEFS += -DSHORT_BOARD_NAME=small-can

include $(BOARDS_DIR)/hellen/hellen-common64.mk
