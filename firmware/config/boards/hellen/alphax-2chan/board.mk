# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARDS_DIR)/hellen/alphax-2chan/board_configuration.cpp
BOARDINC = $(BOARDS_DIR)/hellen/alphax-2chan

# Set this if you want a default engine type other than normal alphax-2chan
ifeq ($(DEFAULT_ENGINE_TYPE),)
  DEFAULT_ENGINE_TYPE = -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS
endif

# 144 package MCU
ifeq ($(LED_CRITICAL_ERROR_BRAIN_PIN),)
  LED_CRITICAL_ERROR_BRAIN_PIN = -DLED_CRITICAL_ERROR_BRAIN_PIN=H144_LED1_RED
endif

DDEFS += -DEFI_MAIN_RELAY_CONTROL=TRUE

DDEFS += -DTS_NO_SECONDARY

# Add them all together
DDEFS += -DEFI_USE_OSC=TRUE -DFIRMWARE_ID=\"AlphaX-2chan\" $(DEFAULT_ENGINE_TYPE) $(LED_CRITICAL_ERROR_BRAIN_PIN) $(LED_COMMUNICATION_BRAIN_PIN)
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE
DDEFS += -DEFI_ICU_INPUTS=FALSE -DHAL_TRIGGER_USE_PAL=TRUE
# todo: is it broken?
DDEFS += -DEFI_LOGIC_ANALYZER=FALSE

# Enable serial pins on expansion header
DDEFS += -DEFI_CONSOLE_TX_BRAIN_PIN=GPIOD_6 -DEFI_CONSOLE_RX_BRAIN_PIN=GPIOD_5 -DTS_PRIMARY_PORT=UARTD2 -DSTM32_UART_USE_USART2=1

include $(BOARDS_DIR)/hellen/hellen-common.mk

