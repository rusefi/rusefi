# List of all the board related files.
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp

DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::I1

# Specific MCU
#CHIBIOS_MCU_TYPE = STM32F746xx
# This is almost the same, but STM32F746xx is already defined in board.h file in ChibiOS
# Defining STM32F746xx here will cause redefined error during compilation.
CHIBIOS_MCU_TYPE = STM32F745xx
# Stock board.c from ChibiOS
BOARD_C = $(CHIBIOS)/os/hal/boards/ST_STM32F746G_DISCOVERY/board.c
# Stock board.h from ChibiOS
BOARDINC = $(CHIBIOS)/os/hal/boards/ST_STM32F746G_DISCOVERY/

# Default F7 linker script is not compatible
LDSCRIPT = $(PROJECT_DIR)/hw_layer/ports/stm32/stm32f7/STM32F746xG.ld

# Save some RAM until we enable SDRAM.
DDEFS += -DEFI_LUA=FALSE

# USB OTG FS connector:
DDEFS += -DEFI_USB_SERIAL_DM=Gpio::A11
DDEFS += -DEFI_USB_SERIAL_DP=Gpio::A12
DDEFS += -DEFI_USB_AF=10

DDEFS += -DEFI_DYNO_VIEW=FALSE

# TODO: Enable ethernet
#LWIP = yes
#ALLOW_SHADOW = yes
#DDEFS += -DCH_CFG_USE_DYNAMIC=TRUE
#DDEFS += -DEFI_ETHERNET=TRUE

BUNDLE_OPENOCD = yes

DDEFS += -DFIRMWARE_ID=\"f746\"
DDEFS += -DDEFAULT_ENGINE_TYPE=engine_type_e::MINIMAL_PINS
DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_F746_DISCOVERY
