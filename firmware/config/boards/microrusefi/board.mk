# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARDS_DIR)/microrusefi/board_configuration.cpp

# Target processor details
ifeq ($(PROJECT_CPU),ARCH_STM32F4)
  BOARDINC  = $(BOARDS_DIR)/microrusefi
  BOARDINC += $(PROJECT_DIR)/config/stm32f4ems	# For board.h
  BOARDINC += $(BOARDS_DIR)/microrusefi # For knock_config.h
else
  BOARDINC += $(BOARDS_DIR)/microrusefi # For knock_config.h
endif

# see also openblt/board.mk STATUS_LED
DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::E3

# *TEMPORARY* breaking TTL thus breaking Bluetooth for microRusEFI in order to enable SPI3 for SD card
# *TODO* need to give people the horrible choice between Bluetooth via TTL or SD card via SPI :( horrible choice 
# PB10/PB11 uses UART3 peripheral and J12/J13 on MRE
# we also have PC10/PC11 exposed on J4 but that's same UART3
DDEFS += -DEFI_CONSOLE_TX_BRAIN_PIN=Gpio::B10 -DEFI_CONSOLE_RX_BRAIN_PIN=Gpio::B11

# on MRE 0.6.0 we have SD card on SPI2 which shared channel 3 with USART3
# todo: enable serial which would not DMA thus not conflict?
DDEFS += -DSTM32_UART_USE_USART3=FALSE -DHAL_USE_UART=FALSE
DDEFS += -DEFI_USE_UART_DMA=FALSE -DTS_NO_PRIMARY=TRUE

# maybe a way to disable SPI2 privately
#DDEFS += -DSTM32_SPI_USE_SPI2=FALSE

DDEFS += -DEFI_CJ125=FALSE -DBOARD_L9779_COUNT=0 -DEFI_HD44780_LCD=FALSE -DEFI_LCD=FALSE

DDEFS += -DFIRMWARE_ID=\"microRusEFI\"
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE
DDEFS += $(VAR_DEF_ENGINE_TYPE)

# We are running on microRusEFI hardware!
DDEFS += -DHW_MICRO_RUSEFI=1

ifeq ($(PROJECT_CPU),ARCH_STM32F7)
	DDEFS += -DSHORT_BOARD_NAME=mre_f7
else ifeq ($(PROJECT_CPU),ARCH_STM32F4)
	DDEFS += -DSHORT_BOARD_NAME=mre_f4
else
$(error Unsupported PROJECT_CPU for microRusEFI: [$(PROJECT_CPU)])
endif
