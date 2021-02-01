# List of all the board related files.
BOARDSRC = $(CHIBIOS)/os/hal/boards/ST_NUCLEO144_F767ZI/board.c
BOARDCPPSRC =  $(PROJECT_DIR)/config/boards/proteus/board_configuration.cpp \
				$(PROJECT_DIR)/config/boards/proteus/adc_hack.cpp
BOARDINC = $(PROJECT_DIR)/config/boards/proteus

# Target processor details
ifeq ($(PROJECT_CPU),ARCH_STM32F4)
  MCU_DEFS  = -DSTM32F407xx
  BOARDSRC  = $(CHIBIOS)/os/hal/boards/ST_STM32F4_DISCOVERY/board.c
  BOARDINC += $(PROJECT_DIR)/config/stm32f4ems	# For board.h
  BOARDINC += $(BOARDS_DIR)/st_stm32f4
  LDSCRIPT  = $(BOARDS_DIR)/prometheus/STM32F405xG.ld
else
  MCU_DEFS  = -DSTM32F767xx
  BOARDSRC  = $(CHIBIOS)/os/hal/boards/ST_NUCLEO144_F767ZI/board.c
  BOARDINC += $(BOARDS_DIR)/nucleo_f767		# For board.h
  BOARDINC += $(PROJECT_DIR)/config/stm32f7ems	# efifeatures/halconf/chconf.h
  LDSCRIPT  = $(BOARDS_DIR)/nucleo_f767/STM32F76xxI.ld
  CONFDIR   = config/stm32f4ems
  PROTEUS_LEGACY = TRUE
endif

# Override DEFAULT_ENGINE_TYPE
DDEFS += $(MCU_DEFS) -DEFI_USE_OSC=TRUE
DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=GPIOE_3
DDEFS += -DFIRMWARE_ID=\"proteus\" -DDEFAULT_ENGINE_TYPE=PROTEUS_DEFAULTS
DDEFS += -DEFI_ICU_INPUTS=FALSE -DHAL_TRIGGER_USE_PAL=TRUE
DDEFS += -DEFI_LOGIC_ANALYZER=FALSE
# todo: refactor HAL_VSS_USE_PAL
DDEFS += -DHAL_VSS_USE_PAL=TRUE
DDEFS += -DSTM32_ADC_USE_ADC3=TRUE
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE
DDEFS += -DEFI_CONSOLE_TX_BRAIN_PIN=GPIO_UNASSIGNED -DEFI_CONSOLE_RX_BRAIN_PIN=GPIO_UNASSIGNED

# We are running on Proteus hardware!
DDEFS += -DHW_PROTEUS=1

# Proteus <=v0.2 needs ADC hack - vbatt is on ADC3
ifeq ($(PROTEUS_LEGACY),TRUE)
	DDEFS +=  -DUSE_ADC3_VBATT_HACK
endif

# Shared variables
ALLCSRC   += $(BOARDSRC)
ALLCPPSRC += $(BOARDCPPSRC)
ALLINC    += $(BOARDINC)
