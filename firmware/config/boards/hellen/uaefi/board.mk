# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp



#no mux on mm100

# Add them all together
DDEFS += -DFIRMWARE_ID=\"uaefi\" $(VAR_DEF_ENGINE_TYPE)
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE
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

DDEFS += -DUSB_DESCRIPTOR_B_LENGTH=24
DDEFS += -DUSB_DESCRIPTOR_STRING_CONTENT="'r', 0, 'u', 0, 's', 0, 'E', 0, 'F', 0, 'I', 0, ' ', 0, 'u', 0, 'a', 0, 'E', 0, 'F', 0, 'I', 0"

