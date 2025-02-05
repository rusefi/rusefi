

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp












ONBOARD_MEMS_TYPE=LIS2DH12

include $(BOARDS_DIR)/hellen/hellen-common-mega144.mk






DDEFS += -DBOOT_BACKDOOR_ENTRY_TIMEOUT_MS=0

DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_ALPHAX_GOLD

