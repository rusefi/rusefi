

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp

# Keep flash headroom; use the matching INI from the firmware bundle.
DDEFS += -DEFI_EMBED_INI_MSD=FALSE


# This board has two tle9104
DDEFS += -DBOARD_TLE9104_COUNT=2









ONBOARD_MEMS_TYPE=LIS2DH12

include $(BOARDS_DIR)/hellen/hellen-common100.mk






DDEFS += -DBOOT_BACKDOOR_ENTRY_TIMEOUT_MS=0

DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_ALPHAX_SILVER

DDEFS += $(PRIMARY_COMMUNICATION_PORT_USART2)
