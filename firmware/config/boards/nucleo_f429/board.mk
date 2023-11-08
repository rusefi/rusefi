# List of all the board related files.
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp
DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::B14

# Enable ethernet
LWIP = yes
DDEFS += -DEFI_ETHERNET=TRUE

# This is an F429!
IS_STM32F429 = yes

SHORT_BOARD_NAME = stm32f429_nucleo

DDEFS += -DFIRMWARE_ID=\"stm32f429_nucleo\"
DDEFS += -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS
