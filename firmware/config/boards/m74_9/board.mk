# List of all the board related files.
# The board's C++ sources need the full firmware (sensors, engine, MFS
# storage, trigger decoding) - the OpenBLT bootloader does not use any of
# them: it only needs board.c (early GPIO/clock init) and the ChibiOS
# drivers. Compiling them into the bootloader pulls the whole runtime in.
ifneq ($(IS_RE_BOOTLOADER),yes)
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp \
  $(BOARD_DIR)/m74_9_can.cpp \
  $(BOARD_DIR)/m74_9_lin.cpp \
  $(BOARD_DIR)/m74_9_tooth_diag.cpp \
  $(BOARD_DIR)/m74_9_vr_model.cpp \
  $(BOARD_DIR)/../at_start_f435/board_storage.cpp
else
BOARDCPPSRC = $(BOARD_DIR)/board_bootloader.cpp
endif

DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::Unassigned

IS_AT32F435 = yes

# board.c from this directory
BOARD_C = $(BOARD_DIR)/board.c
# board.h from this directory
BOARDINC = $(BOARD_DIR)

#This board has no USB wired out
DDEFS += -DSTM32_USB_USE_OTG1=FALSE
DDEFS += -DSTM32_USB_USE_OTG2=FALSE

DDEFS += -DBOARD_L9779_COUNT=1
DDEFS += -DBOARD_TLE9201_COUNT=1
DDEFS += -DSTM32_SPI_USE_SPI2=TRUE

# OpenBLT over CAN1 (PG0 RX / PG1 TX per the tune; transceiver is always-on).
# The running app also listens on the OpenBLT CAN IDs (canOpenBLT=yes in the
# tune) and reboots into the bootloader when the host starts an XCP session.
# RS232 transport is disabled: this board has no USB and the bootloader's
# RS232 layer is USB-CDC only.
DDEFS += -DBOOT_COM_RS232_ENABLE=0
DDEFS += -DBOOT_COM_CAN_CHANNEL_INDEX=0
DDEFS += -DOPENBLT_CAN_RX_PORT=GPIOG
DDEFS += -DOPENBLT_CAN_RX_PIN=0
DDEFS += -DOPENBLT_CAN_TX_PORT=GPIOG
DDEFS += -DOPENBLT_CAN_TX_PIN=1
DDEFS += -DBOOT_BACKDOOR_ENTRY_TIMEOUT_MS=1000

# Software knock: the board has a dedicated knock input (connector AA3 ->
# onboard conditioner -> PA0 = ADC1 IN0). ADC1 is shared with the slow
# sampling (the knock window steals the ADC from the slow background chain),
# see knock_config.h, onStartKnockSampling() and knockCompletionCallback() in
# stm32_adc_v2.cpp.
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE

# This board has no storage
DDEFS += -DEFI_FILE_LOGGING=FALSE
DDEFS += -DEFI_STORAGE_SD=FALSE
USE_FATFS = no

# Configuration directorys
CONFDIR = $(PROJECT_DIR)/hw_layer/ports/at32/at32f4/cfg

# This board uses ChibiOS MFS driver on internal flash
DDEFS += -DHAL_USE_EFL=TRUE
DDEFS += -DEFI_STORAGE_INT_FLASH=FALSE
include $(PROJECT_DIR)/hw_layer/ports/stm32/use_higher_level_flash_api.mk

# Hardware watchdog: the app re-configures the IWDG to ~4 s in
# m74_9_boardInitHardware and feeds it from the 20 Hz slow callback. The
# BOOTLOADER must NOT arm the IWDG on this board (its ~410 ms real window
# expires before the app reaches boardInit -> reset loop), so this define
# applies to the app build only; the bootloader build (IS_RE_BOOTLOADER)
# gets its own HAL_USE_WDG value from firmware/bootloader/Makefile.
ifneq ($(IS_RE_BOOTLOADER),yes)
  DDEFS += -DHAL_USE_WDG=TRUE
endif

DDEFS += -DFIRMWARE_ID=\"m74_9\"
DDEFS += -DDEFAULT_ENGINE_TYPE=engine_type_e::MINIMAL_PINS
DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_M74_9

DDEFS += -DEFI_BACKUP_SRAM=FALSE
DDEFS += -DEFI_STORAGE_INT_FLASH=FALSE
