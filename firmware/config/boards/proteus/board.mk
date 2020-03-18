# List of all the board related files.
BOARDSRC = $(CHIBIOS)/os/hal/boards/ST_NUCLEO144_F767ZI/board.c
BOARDSRC_CPP =  $(PROJECT_DIR)/config/boards/proteus/board_configuration.cpp \
				$(PROJECT_DIR)/config/boards/proteus/adc_hack.cpp

# Target processor details
ifeq ($(PROJECT_CPU),ARCH_STM32F4)
  MCU_DEFS = -DSTM32F407xx
  BOARDSRC = $(CHIBIOS)/os/hal/boards/ST_STM32F4_DISCOVERY/board.c
  BOARDINC = $(BOARDS_DIR)/microrusefi
  BOARDINC += $(PROJECT_DIR)/config/stm32f4ems	# For board.h
  BOARDINC += $(BOARDS_DIR)/st_stm32f4
  LDSCRIPT= $(BOARDS_DIR)/prometheus/STM32F405xG.ld
else
  MCU_DEFS = -DSTM32F767xx
  BOARDSRC = $(CHIBIOS)/os/hal/boards/ST_NUCLEO144_F767ZI/board.c
  BOARDINC = $(BOARDS_DIR)/nucleo_f767		# For board.h
  BOARDINC += $(PROJECT_DIR)/config/stm32f7ems	# efifeatures/halconf/chconf.h
  LDSCRIPT= $(BOARDS_DIR)/nucleo_f767/STM32F76xxI.ld
endif




# Override DEFAULT_ENGINE_TYPE
DDEFS += $(MCU_DEFS) -DEFI_USE_OSC=TRUE -DEFI_FATAL_ERROR_PIN=GPIOE_3 -DFIRMWARE_ID=\"proteus\" -DDEFAULT_ENGINE_TYPE=PROTEUS -DUSE_ADC3_VBATT_HACK -DSTM32_ADC_USE_ADC3=TRUE -DEFI_INCLUDE_ENGINE_PRESETS=FALSE
