# List of all the board related files.

BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp

DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::B14

# Enable ethernet
LWIP = yes
include $(PROJECT_DIR)/controllers/modules/ethernet_console/ethernet_console.mk

DDEFS += -DFIRMWARE_ID=\"nucleo_h743\"
DDEFS += -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS
