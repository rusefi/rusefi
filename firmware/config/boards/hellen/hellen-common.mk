# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC += $(BOARDS_DIR)/hellen/hellen_common.cpp \
    $(BOARDS_DIR)/hellen/hellen_board_id.cpp

DDEFS += -DLED_ERROR_BRAIN_PIN_MODE=INVERTED_OUTPUT
DDEFS += -DLED_RUNING_BRAIN_PIN_MODE=INVERTED_OUTPUT
DDEFS += -DLED_WARNING_BRAIN_PIN_MODE=INVERTED_OUTPUT
DDEFS += -DLED_COMMUNICATION_BRAIN_PIN_MODE=INVERTED_OUTPUT

# We are running on Hellen-One hardware!
DDEFS += -DHW_HELLEN=1

DDEFS += -DTS_NO_SECONDARY=TRUE
