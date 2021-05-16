include $(HW_LAYER_DIR)/ports/stm32/stm32_common.mk

DDEFS += -DSTM32H743xx
MCU = cortex-m7
LDSCRIPT =$(HW_PORT_DIR)/STM32H743xI.ld
ALLCSRC += $(CHIBIOS)/os/hal/boards/ST_NUCLEO144_H743ZI/board.c
CONFDIR = $(HW_PORT_DIR)/cfg

HW_PORTS_SRC += \
	$(HW_PORT_DIR)/stm32h7xx_hal_flash.c \
	$(HW_PORT_DIR)/stm32h7xx_hal_flash_ex.c

HW_PORTS_CPP += \
	$(HW_PORT_DIR)/mpu_util.cpp \
	$(HW_LAYER_DIR)/ports/stm32/stm32_adc_v4.cpp \

# TODO: remove, for efifeatures.h
HW_PORTS_INC += \
	$(PROJECT_DIR)/config/stm32h7ems
