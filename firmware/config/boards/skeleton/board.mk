# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp

BOARDINC  = $(BOARD_DIR)

DDEFS += -DDEFAULT_ENGINE_TYPE=DEFAULT_FRANKENSO
# Add them all together
DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::E3 -DFIRMWARE_ID=\"skeleton\"
