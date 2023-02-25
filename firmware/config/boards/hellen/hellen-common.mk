# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC += $(BOARDS_DIR)/hellen/hellen_common.cpp \
    $(BOARDS_DIR)/hellen/hellen_board_id.cpp

DDEFS += -DLED_ERROR_BRAIN_PIN_MODE=OM_INVERTED
DDEFS += -DLED_RUNING_BRAIN_PIN_MODE=OM_INVERTED
DDEFS += -DLED_WARNING_BRAIN_PIN_MODE=OM_INVERTED
DDEFS += -DLED_COMMUNICATION_BRAIN_PIN_MODE=OM_INVERTED

# We are running on Hellen-One hardware!
DDEFS += -DHW_HELLEN=1
