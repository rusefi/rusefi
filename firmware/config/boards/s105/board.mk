HALCONFDIR = $(BOARD_DIR)

# List of all the board related files.
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp

# This is S105 board
DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_S105

# This board has no LSE and HSE oscillators
DDEFS += -DSTM32_HSE_ENABLED=FALSE
DDEFS += -DENABLE_AUTO_DETECT_HSE=FALSE

# This board has 512K STM32F407
DDEFS += -DMIN_FLASH_SIZE=512

#This board has no LED
DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::Unassigned
# and no LCD
DDEFS += -DEFI_HD44780_LCD=FALSE

# No Lua support as we are limited in RAM and ROM
DDEFS += -DEFI_LUA=FALSE

#This board has no USB wired out
DDEFS += -DSTM32_USB_USE_OTG1=FALSE
DDEFS += -DSTM32_USB_USE_OTG2=FALSE

# This board has no storage
DDEFS += -DEFI_FILE_LOGGING=FALSE
USE_FATFS = no

# TS serial
DDEFS += -DSTM32_SERIAL_USE_USART1=TRUE
DDEFS += -DTS_PRIMARY_UxART_PORT=SD1 -DEFI_TS_PRIMARY_IS_SERIAL=TRUE
# WARNING: those are USB pins on discovery or rusEFI brain board so make sure to power those NOT via USB
DDEFS += -DEFI_CONSOLE_TX_BRAIN_PIN=Gpio::A9 -DEFI_CONSOLE_RX_BRAIN_PIN=Gpio::A10

DDEFS += -DFIRMWARE_ID=\"s105\"
DDEFS += -DDEFAULT_ENGINE_TYPE=engine_type_e::MINIMAL_PINS

# Required include directories
# Shared variables
# Add board's directory first in include dir list so files in board directory will be included instead of default
ALLINC += $(BOARDINC)
