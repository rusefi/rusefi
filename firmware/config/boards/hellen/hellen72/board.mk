# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARDS_DIR)/hellen/hellen72/board_configuration.cpp
BOARDINC = $(BOARDS_DIR)/hellen/hellen72

# Target processor details
ifeq ($(PROJECT_CPU),ARCH_STM32F4)
  MCU_DEFS  = -DSTM32F427xx
  BOARDSRC  = $(CHIBIOS)/os/hal/boards/ST_STM32F4_DISCOVERY/board.c
  BOARDINC += $(PROJECT_DIR)/config/stm32f4ems	# For board.h
  BOARDINC += $(BOARDS_DIR)/st_stm32f4
  LDSCRIPT  = $(BOARDS_DIR)/prometheus/STM32F405xG.ld
else
  # todo: add support for STM32H7
  MCU_DEFS  = -DSTM32H743xx
  BOARDSRC  = $(CHIBIOS)/os/hal/boards/ST_NUCLEO144_H743ZI/board.c
  CONFDIR   = config/stm32h7ems
  BOARDINC += $(CHIBIOS)/os/hal/boards/ST_NUCLEO144_H743ZI # For board.h
  BOARDINC += $(PROJECT_DIR)/config/stm32h7ems	# efifeatures/halconf/chconf.h
  LDSCRIPT  = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/ld/STM32H743xI.ld
endif

# Set this if you want a default engine type other than normal Hellen72
ifeq ($(DEFAULT_ENGINE_TYPE),)
  DEFAULT_ENGINE_TYPE = -DDEFAULT_ENGINE_TYPE=HELLEN_NB2
endif

ifeq ($(LED_CRITICAL_ERROR_BRAIN_PIN),)
  LED_CRITICAL_ERROR_BRAIN_PIN = -DLED_CRITICAL_ERROR_BRAIN_PIN=GPIOH_8
endif

ifeq ($(LED_COMMUNICATION_BRAIN_PIN),)
  LED_COMMUNICATION_BRAIN_PIN = -DEFI_COMMUNICATION_PIN=GPIOH_10
endif

# Add them all together
DDEFS += $(MCU_DEFS) -DEFI_USE_OSC=TRUE -DFIRMWARE_ID=\"hellen72\" $(DEFAULT_ENGINE_TYPE) $(LED_CRITICAL_ERROR_BRAIN_PIN) $(LED_COMMUNICATION_BRAIN_PIN)
# -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE
DDEFS += -DEFI_ICU_INPUTS=FALSE -DHAL_TRIGGER_USE_PAL=TRUE -DHAL_VSS_USE_PAL=TRUE 
# todo: is it broken?
DDEFS += -DEFI_LOGIC_ANALYZER=FALSE

# We are running on Hellen-One hardware!
DDEFS += -DHW_HELLEN=1

# Shared variables
ALLCSRC   += $(BOARDSRC)
ALLCPPSRC += $(BOARDCPPSRC)
ALLINC    += $(BOARDINC)
