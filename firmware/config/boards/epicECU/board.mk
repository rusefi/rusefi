BOARDCPPSRC =  $(BOARD_DIR)/board_configuration.cpp

#    $(BOARD_DIR)/firmware/pinouts.cpp \

BOARDINC += $(BOARD_DIR)/generated/controllers/generated

include $(PROJECT_DIR)/hw_layer/ports/stm32/2mb_flash.mk

# defines SHORT_BOARD_NAME
include $(BOARD_DIR)/meta-info.env

# reduce memory usage monitoring
DDEFS += -DRAM_UNUSED_SIZE=100

# DDEFS += -DEFI_ONBOARD_MEMS=TRUE
# DDEFS += -DEFI_ONBOARD_MEMS_LIS3DSH=TRUE

# assign critical LED unassigned
DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::Unassigned


DDEFS += -DEFI_FLASH_WRITE_THREAD=FALSE

DDEFS += -DSTM32_CAN_USE_CAN3=TRUE

# we do not have much Lua RAM, let's drop some fancy functions
DDEFS += -DWITH_LUA_CONSUMPTION=FALSE
DDEFS += -DWITH_LUA_PID=FALSE
DDEFS += -DWITH_LUA_STOP_ENGINE=FALSE

# TS_SECONDARY_UxART_PORT
DDEFS += -DSTM32_SERIAL_USE_USART3=TRUE -DSTM32_SERIAL_USART3_PRIORITY=2
DDEFS += -DTS_SECONDARY_UxART_PORT=SD3 -DEFI_TS_SECONDARY_IS_SERIAL=TRUE

# this is here just to assert compilation
DDEFS += -DALLOW_JUMP_WITH_IGNITION_VOLTAGE=FALSE

# knock
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE
