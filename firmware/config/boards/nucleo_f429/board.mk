# List of all the board related files.

# this board has 2Mb chip
include $(PROJECT_DIR)/hw_layer/ports/stm32/2mb_flash.mk

BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp
DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::B14

# Enable ethernet
EFI_ETHERNET = yes

# This is an F429!
IS_STM32F429 = yes

BUNDLE_OPENOCD = yes

DDEFS += -DFIRMWARE_ID=\"nucleo_f429\"
DDEFS += -DDEFAULT_ENGINE_TYPE=engine_type_e::MINIMAL_PINS
DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_NUCLEO_F429
