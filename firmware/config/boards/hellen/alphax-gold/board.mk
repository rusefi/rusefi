

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp


# This board has four tle9104
DDEFS += -DBOARD_TLE9104_COUNT=4









ONBOARD_MEMS_TYPE=LIS2DH12

include $(BOARDS_DIR)/hellen/hellen-common-mega144.mk






DDEFS += -DBOOT_BACKDOOR_ENTRY_TIMEOUT_MS=0

# let's start quickly
DDEFS += -DHW_HELLEN_SKIP_BOARD_TYPE=TRUE
DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_ALPHAX_GOLD

