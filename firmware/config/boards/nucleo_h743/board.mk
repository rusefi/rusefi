# List of all the board related files.

DDEFS = -DTS_NO_PRIMARY=0 \
	-DTS_PRIMARY_PORT=SD3 \
	-DEFI_CONSOLE_TX_BRAIN_PIN=Gpio::D8 \
	-DEFI_CONSOLE_RX_BRAIN_PIN=Gpio::D9 \


# Shared variables
ALLCPPSRC += $(PROJECT_DIR)/config/boards/nucleo_h743/board_configuration.cpp

