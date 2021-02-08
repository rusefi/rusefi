include $(PROJECT_DIR)/hw_layer/ports/stm32/stm32_common.mk

HW_LAYER_EMS += $(PROJECT_DIR)/hw_layer/ports/stm32/stm32f4/stm32f4xx_hal_flash.c \
				$(PROJECT_DIR)/hw_layer/ports/stm32/stm32f4/stm32f4xx_hal_flash_ex.c \
               
HW_LAYER_EMS_CPP += $(PROJECT_DIR)/hw_layer/ports/stm32/stm32f4/mpu_util.cpp

DDEFS += -DSTM32F407xx
LDSCRIPT = $(PROJECT_DIR)/hw_layer/ports/stm32/stm32f4/STM32F405xG.ld
ALLCSRC += $(CHIBIOS)/os/hal/boards/ST_STM32F4_DISCOVERY/board.c
CONFDIR = $(PROJECT_DIR)/hw_layer/ports/stm32/stm32f4/cfg

# TODO: remove, for efifeatures.h
ALLINC += $(PROJECT_DIR)/config/stm32f4ems
