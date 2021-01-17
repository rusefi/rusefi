# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARDS_DIR)/microrusefi/board_configuration.cpp

# Target processor details
ifeq ($(PROJECT_CPU),ARCH_STM32F4)
  MCU_DEFS  = -DSTM32F407xx
  BOARDSRC  = $(CHIBIOS)/os/hal/boards/ST_STM32F4_DISCOVERY/board.c
  BOARDINC  = $(BOARDS_DIR)/microrusefi
  BOARDINC += $(PROJECT_DIR)/config/stm32f4ems	# For board.h
  BOARDINC += $(BOARDS_DIR)/st_stm32f4
  BOARDINC += $(BOARDS_DIR)/microrusefi # For knock_config.h
  LDSCRIPT  = $(BOARDS_DIR)/prometheus/STM32F405xG.ld
else
  MCU_DEFS  = -DSTM32F767xx
  BOARDSRC  = $(CHIBIOS)/os/hal/boards/ST_NUCLEO144_F767ZI/board.c
  CONFDIR   = config/stm32f7ems
  BOARDINC  = $(BOARDS_DIR)/nucleo_f767		# For board.h
  BOARDINC += $(PROJECT_DIR)/config/stm32f7ems	# efifeatures/halconf/chconf.h
  BOARDINC += $(BOARDS_DIR)/microrusefi # For knock_config.h
  LDSCRIPT  = $(BOARDS_DIR)/nucleo_f767/STM32F76xxI.ld
endif

# Set this if you want a default engine type other than normal MRE
ifeq ($(DEFAULT_ENGINE_TYPE),)
  DEFAULT_ENGINE_TYPE = -DDEFAULT_ENGINE_TYPE=MICRO_RUS_EFI
endif

ifeq ($(LED_CRITICAL_ERROR_BRAIN_PIN),)
  LED_CRITICAL_ERROR_BRAIN_PIN = -DLED_CRITICAL_ERROR_BRAIN_PIN=GPIOE_3
endif


# *TEMPORARY* breaking TTL thus breaking Bluetooth for microRusEFI in order to enable SPI3 for SD card
# *TODO* need to give people the horrible choice between Bluetooth via TTL or SD card via SPI :( horrible choice 
EFI_CONSOLE_TTL_PINS = -DEFI_CONSOLE_TX_BRAIN_PIN=GPIOB_10 -DEFI_CONSOLE_RX_BRAIN_PIN=GPIOB_11


# Add them all together
DDEFS += $(MCU_DEFS) -DEFI_USE_OSC=TRUE -DFIRMWARE_ID=\"microRusEFI\" $(DEFAULT_ENGINE_TYPE) $(LED_CRITICAL_ERROR_BRAIN_PIN) $(EFI_CONSOLE_TTL_PINS) -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE 

# We are running on microRusEFI hardware!
DDEFS += -DHW_MICRO_RUSEFI=1

# Shared variables
ALLCSRC   += $(BOARDSRC)
ALLCPPSRC += $(BOARDCPPSRC)
ALLINC    += $(BOARDINC)
