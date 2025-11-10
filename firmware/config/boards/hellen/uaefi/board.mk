# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp

ifeq ($(PROJECT_CPU),ARCH_STM32F7)
	DDEFS += -DLUA_RX_MAX_FILTER_COUNT=96
endif

#no mux on mm100

# Add them all together
DDEFS += -DFIRMWARE_ID=\"uaefi\" $(VAR_DEF_ENGINE_TYPE)

DDEFS += -DEFI_SKIP_BOR=TRUE

#Knock is available on F4 and F7
ifeq ($(PROJECT_CPU),ARCH_STM32H7)
	# Default H743 linker script is not compatible
	LDSCRIPT = $(PROJECT_DIR)/hw_layer/ports/stm32/stm32h7/STM32H723xG_ITCM64k.ld
	# Do not use HSE autodetection
	DDEFS += -DSTM32_HSECLK=20000000
	DDEFS += -DENABLE_AUTO_DETECT_HSE=FALSE
else
	#Knock is available on F4 and F7 only
	DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE
endif

# EGT chip
DDEFS += -DEFI_MAX_31855=TRUE

DDEFS += -DHELLEN_BOARD_ID_DEBUG

# on the one hand we do not use boardID YET and it takes 350ms, on the other hand who knows what the future would bring
# as long as we only have one boardID for all units the only consumer of boardID is HW QC process
# so far all uaEFI boards are completely compatible
DDEFS += -DHW_HELLEN_SKIP_BOARD_TYPE=TRUE
DDEFS += -DSTATIC_BOARD_ID=BOARD_ID_UAEFI_B

DDEFS += -DHW_HELLEN_UAEFI=1
DDEFS += -DDIAG_5VP_PIN=Gpio::MM100_SPI3_MOSI

DDEFS += -DSTM32_ICU_USE_TIM1=TRUE -DSTM32_PWM_USE_TIM1=FALSE
DDEFS += -DEFI_SENT_SUPPORT=TRUE

ONBOARD_MEMS_TYPE=LIS2DH12

include $(BOARDS_DIR)/hellen/hellen-common100.mk

# we do not have much Lua RAM, let's drop some fancy functions
DDEFS += -DWITH_LUA_CONSUMPTION=FALSE
DDEFS += -DWITH_LUA_PID=FALSE
DDEFS += -DWITH_LUA_STOP_ENGINE=FALSE

DDEFS += $(PRIMARY_COMMUNICATION_PORT_USART2)

DDEFS += -DUSB_DESCRIPTOR_B_LENGTH=26
DDEFS += -DUSB_DESCRIPTOR_STRING_CONTENT="'r', 0, 'u', 0, 's', 0, 'E', 0, 'F', 0, 'I', 0, ' ', 0, 'u', 0, 'a', 0, 'E', 0, 'F', 0, 'I', 0"

DDEFS += -DBOARD_SERIAL="\"000000000000000000000000\""

# CAND1
DDEFS += -DBOOT_COM_CAN_CHANNEL_INDEX=0
DDEFS += -DOPENBLT_CAN_RX_PORT=GPIOD
DDEFS += -DOPENBLT_CAN_RX_PIN=0
DDEFS += -DOPENBLT_CAN_TX_PORT=GPIOD
DDEFS += -DOPENBLT_CAN_TX_PIN=1
