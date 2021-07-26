# List of all the board related files.
BOARDCPPSRC =  $(PROJECT_DIR)/config/boards/proteus/board_configuration.cpp \
				$(PROJECT_DIR)/config/boards/proteus/adc_hack.cpp

BOARDINC = $(PROJECT_DIR)/config/boards/proteus

ifeq ($(PROJECT_CPU),ARCH_STM32F4)
  IS_STM32F429 = yes
endif

# Set this if you want a default engine type other than normal Proteus
ifeq ($(DEFAULT_ENGINE_TYPE),)
  DEFAULT_ENGINE_TYPE = -DDEFAULT_ENGINE_TYPE=PROTEUS_DEFAULTS
endif

DDEFS += -DEFI_USE_OSC=TRUE
DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=GPIOE_3
DDEFS += -DFIRMWARE_ID=\"proteus\" $(DEFAULT_ENGINE_TYPE)

DDEFS += -DEFI_ICU_INPUTS=FALSE -DHAL_TRIGGER_USE_PAL=TRUE
DDEFS += -DEFI_LOGIC_ANALYZER=FALSE
# todo: refactor HAL_VSS_USE_PAL
DDEFS += -DHAL_VSS_USE_PAL=TRUE

# This stuff doesn't work on H7 yet
ifneq ($(PROJECT_CPU),ARCH_STM32H7)
	DDEFS += -DSTM32_ADC_USE_ADC3=TRUE
	DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE
endif

# disable hardware serial ports on H7
ifeq ($(PROJECT_CPU),ARCH_STM32F4)
	# Hardware serial port on UART 2 -> PD5/PD6
	DDEFS += -DSTM32_UART_USE_USART2=TRUE
	DDEFS += -DTS_PRIMARY_UART=UARTD2
	DDEFS += -DEFI_CONSOLE_TX_BRAIN_PIN=GPIOD_5 -DEFI_CONSOLE_RX_BRAIN_PIN=GPIOD_6
endif

# We are running on Proteus hardware!
DDEFS += -DHW_PROTEUS=1

# Proteus <=v0.2 needs ADC hack - vbatt is on ADC3
ifeq ($(PROTEUS_LEGACY),TRUE)
	DDEFS +=  -DUSE_ADC3_VBATT_HACK
endif

# Shared variables
ALLCPPSRC += $(BOARDCPPSRC)
ALLINC    += $(BOARDINC)
