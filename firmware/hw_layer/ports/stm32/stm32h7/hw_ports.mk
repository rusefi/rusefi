include $(PROJECT_DIR)/hw_layer/ports/stm32/stm32_common.mk

HW_LAYER_EMS += $(PROJECT_DIR)/hw_layer/ports/stm32/stm32h7/stm32h7xx_hal_flash.c \
				$(PROJECT_DIR)/hw_layer/ports/stm32/stm32h7/stm32h7xx_hal_flash_ex.c

HW_LAYER_EMS_CPP += $(PROJECT_DIR)/hw_layer/ports/stm32/stm32h7/mpu_util.cpp

DDEFS += -DSTM32H743xx
LDSCRIPT = $(PROJECT_DIR)/hw_layer/ports/stm32/stm32h7/STM32H743xI.ld
ALLCSRC += $(CHIBIOS)/os/hal/boards/ST_NUCLEO144_H743ZI/board.c
