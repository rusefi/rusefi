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
# Stock board.h from ChibiOS is pulled in by the board.h wrapper in this folder
# (see the comment there); mcuconf.h here overrides clocks and USB OTG2

# Default F7 linker script is not compatible
LDSCRIPT = $(PROJECT_DIR)/hw_layer/ports/stm32/stm32f7/STM32F746xG.ld

# Save some RAM until we enable SDRAM: EFI_LUA=FALSE is declared in
# prepend.txt and lifted into DDEFS by the Makefile - see [tag:ts_page_table]

# USB: OTG HS with external ULPI PHY (CN13), see mcuconf.h. The FS DM/DP pads
# (EFI_USB_SERIAL_DM/DP) are not used; ULPI pins are claimed in board_configuration.cpp

# microSD slot over SDIO (SDMMC1), see mcuconf.h; replaces the SPI MMC driver
DDEFS += -DEFI_SDC_DEVICE=SDCD1

# ignore fact that this is 1Mb chip in single bank mode
DDEFS += -DEFI_FLASH_IGNORE_SINGLE_BANK=TRUE

DDEFS += -DEFI_DYNO_VIEW=FALSE

# TODO: Enable ethernet
EFI_ETHERNET = yes

BUNDLE_OPENOCD = yes

DDEFS += -DFIRMWARE_ID=\"f746\"
DDEFS += -DDEFAULT_ENGINE_TYPE=engine_type_e::MINIMAL_PINS
DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_F746_DISCOVERY
