include $(HW_LAYER_DIR)/ports/stm32/stm32_common.mk

DDEFS += -DSTM32F407xx
MCU = cortex-m4
LDSCRIPT = $(HW_PORT_DIR)/STM32F4.ld
ALLCSRC += $(CHIBIOS)/os/hal/boards/ST_STM32F4_DISCOVERY/board.c
CONFDIR = $(HW_PORT_DIR)/cfg

# STM32F42x has extra memory, so change some flags so we can use it.
ifeq ($(IS_STM32F429),yes)
	USE_OPT += -Wl,--defsym=STM32F4_HAS_SRAM3=1
	DDEFS += -DEFI_IS_F42x
endif

HW_PORTS_SRC += \
	$(HW_PORT_DIR)/stm32f4xx_hal_flash.c \
	$(HW_PORT_DIR)/stm32f4xx_hal_flash_ex.c \

HW_PORTS_CPP += \
	$(HW_PORT_DIR)/mpu_util.cpp \
	$(HW_LAYER_DIR)/ports/stm32/stm32_adc_v2.cpp \

# TODO: remove, for efifeatures.h
HW_PORTS_INC += \
	$(PROJECT_DIR)/config/stm32f4ems
