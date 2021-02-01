# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARDS_DIR)/skeleton/board_configuration.cpp

# Target processor details
ifeq ($(PROJECT_CPU),ARCH_STM32F4)
  MCU_DEFS  = -DSTM32F407xx
  BOARDSRC  = $(CHIBIOS)/os/hal/boards/ST_STM32F4_DISCOVERY/board.c
  BOARDINC  = $(BOARDS_DIR)/skeleton
  BOARDINC += $(PROJECT_DIR)/config/stm32f4ems	# For board.h
  BOARDINC += $(BOARDS_DIR)/st_stm32f4
  LDSCRIPT  = $(BOARDS_DIR)/prometheus/STM32F405xG.ld
else
  MCU_DEFS  = -DSTM32F767xx
  BOARDSRC  = $(CHIBIOS)/os/hal/boards/ST_NUCLEO144_F767ZI/board.c
  BOARDINC  = $(BOARDS_DIR)/nucleo_f767		# For board.h
  BOARDINC += $(PROJECT_DIR)/config/stm32f7ems	# efifeatures/halconf/chconf.h
  LDSCRIPT  = $(BOARDS_DIR)/nucleo_f767/STM32F76xxI.ld
endif

# Set this if you want a default engine type
ifeq ($(DEFAULT_ENGINE_TYPE),)
  DEFAULT_ENGINE_TYPE = -DDEFAULT_ENGINE_TYPE=MICRO_RUS_EFI
endif

# Add them all together
DDEFS += $(MCU_DEFS) -DEFI_USE_OSC=TRUE -DLED_CRITICAL_ERROR_BRAIN_PIN=GPIOE_3 -DFIRMWARE_ID=\"skeleton\" $(DEFAULT_ENGINE_TYPE)

# Shared variables
ALLCSRC   += $(BOARDSRC)
ALLCPPSRC += $(BOARDCPPSRC)
ALLINC    += $(BOARDINC)
