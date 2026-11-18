include $(PROJECT_DIR)/hw_layer/ports/stm32/stm32_common.mk

HW_LAYER_PORT += $(PROJECT_DIR)/hw_layer/ports/stm32/stm32f7/stm32f7xx_hal_flash.c \
				$(PROJECT_DIR)/hw_layer/ports/stm32/stm32f7/stm32f7xx_hal_flash_ex.c

HW_LAYER_PORT_CPP += $(PROJECT_DIR)/hw_layer/ports/stm32/stm32f7/mpu_util.cpp \
					$(PROJECT_DIR)/hw_layer/ports/stm32/stm32_adc_v2.cpp \
					$(PROJECT_DIR)/hw_layer/ports/stm32/stm32_adc_v2_adc3.cpp \
					$(PROJECT_DIR)/hw_layer/ports/stm32/stm32f7/stm32f7xx_rtc.cpp \

# This MCU has a cache, align functions to a cache line for maximum cache efficiency
USE_OPT += -falign-functions=16

ifeq ($(CHIBIOS_MCU_TYPE),)
	CHIBIOS_MCU_TYPE=STM32F767xx
endif
DDEFS += -D$(CHIBIOS_MCU_TYPE)
MCU = cortex-m7
USE_FPU = hard
# Only STM32F76x/F77x have the double-precision FPU (FPv5-DP). F72x/F73x/F74x/F75x
# have FPv5-SP: building them with fpv5-d16 emits f64 instructions that trap
# as UNDEFINSTR UsageFault (seen as HardFault) on the first double operation.
ifneq ($(filter STM32F722xx STM32F723xx STM32F730xx STM32F732xx STM32F733xx STM32F745xx STM32F746xx STM32F750xx STM32F756xx,$(CHIBIOS_MCU_TYPE)),)
	USE_FPU_OPT = -mfloat-abi=$(USE_FPU) -mfpu=fpv5-sp-d16
else
	USE_FPU_OPT = -mfloat-abi=$(USE_FPU) -mfpu=fpv5-d16
endif
ifeq ($(LDSCRIPT),)
	LDSCRIPT = $(PROJECT_DIR)/hw_layer/ports/stm32/stm32f7/STM32F7.ld
endif
# kludge: while we the very generic ChibiOS board.c we use our custom board.h from current folder!
ifeq ($(BOARD_C),)
	BOARD_C = $(CHIBIOS)/os/hal/boards/ST_NUCLEO144_F767ZI/board.c
endif
ALLCSRC += $(BOARD_C)
CONFDIR = $(PROJECT_DIR)/hw_layer/ports/stm32/stm32f7/cfg

# TODO: remove, for efifeatures.h
ALLINC += $(PROJECT_DIR)/config/stm32f7ems
