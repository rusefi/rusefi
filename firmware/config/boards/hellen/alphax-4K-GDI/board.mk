

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp

include $(PROJECT_DIR)/hw_layer/ports/stm32/2mb_flash.mk

DDEFS += -DEFI_MAIN_RELAY_CONTROL=TRUE
DDEFS += -DEFI_MC33816=TRUE
DDEFS += -DBOARD_TLE9104_COUNT=4
DDEFS += -DLUA_USER_HEAP=92000

DDEFS += -DSTM32_ICU_USE_TIM1=TRUE -DSTM32_PWM_USE_TIM1=FALSE
DDEFS += -DEFI_SENT_SUPPORT=TRUE



# we have hellen board id BOARD_ID_GDI4CHAN_A

DDEFS += -DHW_HELLEN_4K_GDI=1

include $(BOARDS_DIR)/hellen/hellen-common-mega144.mk

