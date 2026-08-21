HW_LAYER_PORT = $(PROJECT_DIR)/hw_layer/ports/stm32/stm32f4/stm32f4xx_hal_flash.c \
				$(PROJECT_DIR)/hw_layer/ports/stm32/stm32f4/stm32f4xx_hal_flash_ex.c
               
HW_LAYER_PORT_CPP += $(PROJECT_DIR)/hw_layer/ports/stm32/stm32f4/mpu_util.cpp \
					$(PROJECT_DIR)/hw_layer/ports/stm32/stm32_adc_v2.cpp \
					$(PROJECT_DIR)/hw_layer/ports/stm32/stm32f4/stm32f4xx_rtc.cpp

MCU = cortex-m4
USE_FPU = hard
LDSCRIPT = $(PROJECT_DIR)/hw_layer/ports/stm32/stm32f4/STM32F4.ld
# kludge: while we the very generic ChibiOS board.c we use our custom board.h from current folder!
ifeq ($(BOARD_C),)
	BOARD_C = $(CHIBIOS)/os/hal/boards/ST_STM32F4_DISCOVERY/board.c
endif
ALLCSRC += $(BOARD_C)

ifeq ($(CONFDIR),)
	CONFDIR = $(PROJECT_DIR)/hw_layer/ports/stm32/stm32f4/cfg
endif

# STM32F42x has extra memory, so change some flags so we can use it.
ifeq ($(IS_STM32F429),yes)
	USE_OPT += -Wl,--defsym=STM32F4_HAS_SRAM3=1
	DDEFS += -DSTM32F429xx -DSTM32F429_MCUCONF
	DDEFS += -DEFI_IS_F42x
else ifeq ($(IS_AT32F435),yes)
	# STM32 flash driver is not compatible
	DDEFS += -DAT32F4XX
	HW_LAYER_PORT =
	# TODO: Artery
	HW_LAYER_PORT_CPP = $(PROJECT_DIR)/hw_layer/ports/stm32/stm32_adc_v2.cpp
	# TODO: Arterys
	LDSCRIPT = $(PROJECT_DIR)/hw_layer/ports/at32/at32f4/AT32F435ZMxx.ld
else
	DDEFS += -DSTM32F407xx
endif

ifeq ($(IS_AT32F435),yes)
	# Add Artery AT32 common stuff
	include $(PROJECT_DIR)/hw_layer/ports/at32/at32_common.mk
else
	# Or STM32 common stuff
	include $(PROJECT_DIR)/hw_layer/ports/stm32/stm32_common.mk
endif

# TODO: remove, for efifeatures.h
ALLINC += $(PROJECT_DIR)/config/stm32f4ems
