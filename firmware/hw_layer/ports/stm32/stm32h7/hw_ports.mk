include $(PROJECT_DIR)/hw_layer/ports/stm32/stm32_common.mk

HW_LAYER_PORT += $(PROJECT_DIR)/hw_layer/ports/stm32/stm32h7/stm32h7xx_hal_flash.c \
				$(PROJECT_DIR)/hw_layer/ports/stm32/stm32h7/stm32h7xx_hal_flash_ex.c

HW_LAYER_PORT_CPP += $(PROJECT_DIR)/hw_layer/ports/stm32/stm32h7/mpu_util.cpp \
					$(PROJECT_DIR)/hw_layer/ports/stm32/stm32_adc_v4.cpp \

# This MCU has a cache, align functions to a cache line for maximum cache efficiency
USE_OPT += -falign-functions=16

ifeq ($(CHIBIOS_MCU_TYPE),)
	CHIBIOS_MCU_TYPE=STM32H743xx
endif
DDEFS += -D$(CHIBIOS_MCU_TYPE)
MCU = cortex-m7
USE_FPU = hard
USE_FPU_OPT = -mfloat-abi=$(USE_FPU) -mfpu=fpv5-d16
ifeq ($(LDSCRIPT),)
	LDSCRIPT = $(PROJECT_DIR)/hw_layer/ports/stm32/stm32h7/STM32H743xI.ld
endif
ALLCSRC += $(CHIBIOS)/os/hal/boards/ST_NUCLEO144_H743ZI/board.c
CONFDIR = $(PROJECT_DIR)/hw_layer/ports/stm32/stm32h7/cfg

# TODO: remove, for efifeatures.h
ALLINC += $(PROJECT_DIR)/config/stm32h7ems
