# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC += $(BOARDS_DIR)/hellen/hellen_common.cpp \
    $(BOARDS_DIR)/hellen/hellen_board_id.cpp

BOARDINC += $(BOARDS_DIR)/hellen

DDEFS += -DLED_PIN_MODE=OM_INVERTED

# We are running on Hellen-One hardware!
DDEFS += -DHW_HELLEN=1
