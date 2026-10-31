
ifeq ($(PROJECT_BOARD),)
ifneq ($(SHORT_BOARD_NAME),)
  PROJECT_BOARD = $(SHORT_BOARD_NAME)
else
  PROJECT_BOARD = f407-discovery
endif
endif

BOARDS_DIR = $(PROJECT_DIR)/config/boards

ifneq ($(META_OUTPUT_ROOT_FOLDER),)
  ALLINC += $(PROJECT_DIR)/$(META_OUTPUT_ROOT_FOLDER)controllers/generated
endif

# allow passing a custom board dir, otherwise generate it based on the board name
ifeq ($(BOARD_DIR),)
	BOARD_DIR = $(BOARDS_DIR)/$(PROJECT_BOARD)
	-include $(BOARD_DIR)/meta-info.env
endif

ifeq ($(PROJECT_CPU),)
  # while building PROJECT_CPU is provided as 'make' command line argument value and we do not seem to be able to change that value
  # looks like 'make clean' is the only consumer of this value?!
  PROJECT_CPU = ARCH_STM32F4
endif

-include $(BOARD_DIR)/config.mk

# Hardware enums: STM32 by default. A platform can provide its own Gpio and
# adc_channel_e headers through HW_PIN_ENUMS_FILE / HW_ADC_ENUMS_FILE in
# meta-info.env (paths relative to BOARD_DIR); the same files feed the
# config generator (rusefi_config.mk, gen_config_board.sh).
ifneq ($(HW_PIN_ENUMS_FILE),)
  HW_PIN_ENUMS = $(BOARD_DIR)/$(HW_PIN_ENUMS_FILE)
  DDEFS += -DEFI_HW_PIN_ENUMS_FILE=\"$(abspath $(HW_PIN_ENUMS))\"
else
  HW_PIN_ENUMS = $(PROJECT_DIR)/controllers/algo/rusefi_hw_stm32_enums.h
endif
ifneq ($(HW_ADC_ENUMS_FILE),)
  HW_ADC_ENUMS = $(BOARD_DIR)/$(HW_ADC_ENUMS_FILE)
  DDEFS += -DEFI_HW_ADC_ENUMS_FILE=\"$(abspath $(HW_ADC_ENUMS))\"
else
  HW_ADC_ENUMS = $(PROJECT_DIR)/controllers/algo/rusefi_hw_adc_enums.h
endif

# Build the generated pin code only if the connector directory exists
ifneq ("$(wildcard $(BOARD_DIR)/connectors/*.yaml)","")
  ALLCPPSRC += $(BOARD_DIR)/connectors/generated_ts_name_by_pin.cpp
endif

# CPU-dependent defs
ifeq ($(PROJECT_CPU),ARCH_STM32F7)
  CPU_STARTUP = startup_stm32f7xx.mk
  # next file is included through Contrib's platform.mk
  #CPU_PLATFORM = $(CHIBIOS)/os/hal/ports/STM32/STM32F7xx/platform.mk
  CPU_PLATFORM = ${CHIBIOS_CONTRIB}/os/hal/ports/STM32/STM32F7xx/platform.mk
  CPU_HWLAYER = ports/stm32/stm32f7
else ifeq ($(PROJECT_CPU),ARCH_STM32F4)
  CPU_STARTUP = startup_stm32f4xx.mk
  # next file is included through Contrib's platform.mk
  #CPU_PLATFORM = $(CHIBIOS)/os/hal/ports/STM32/STM32F4xx/platform.mk
  CPU_PLATFORM = ${CHIBIOS_CONTRIB}/os/hal/ports/STM32/STM32F4xx/platform.mk
  CPU_HWLAYER = ports/stm32/stm32f4
  # Save startup Lua RAM by registering native functions as scripts use them.
  DDEFS += -DLUA_LAZY_NATIVE_HOOKS=TRUE
else ifeq ($(PROJECT_CPU),ARCH_STM32H7)
  CPU_STARTUP = startup_stm32h7xx.mk
  # TODO: why different platform.mk for different H7?
  ifeq ($(CHIBIOS_MCU_TYPE),STM32H723xx)
    CPU_PLATFORM = $(CHIBIOS)/os/hal/ports/STM32/STM32H7xx/platform_type2.mk
  else
    CPU_PLATFORM = ${CHIBIOS_CONTRIB}/os/hal/ports/STM32/STM32H7xx/platform.mk
  endif
  CPU_HWLAYER = ports/stm32/stm32h7
else ifeq ($(PROJECT_CPU),ARCH_AT32F4)
  CPU_STARTUP = startup_at32f4xx.mk
  CPU_PLATFORM = $(CHIBIOS)/os/hal/ports/AT32/AT32F4xx/platform.mk
  # Reuse STM32F4 port
  CPU_HWLAYER = ports/stm32/stm32f4
else ifneq ("$(wildcard $(BOARD_DIR)/$(PROJECT_CPU).mk)","")
  # the board ships its own platform: PROJECT_CPU names a makefile in the board directory
  include $(BOARD_DIR)/$(PROJECT_CPU).mk
  $(info Using custom CPU_STARTUP_DIR $(CPU_STARTUP_DIR))
  $(info Using custom CPU_PLATFORM $(CPU_PLATFORM))
  $(info Using custom CPU_HWLAYER $(CPU_HWLAYER))
else ifeq ($(PROJECT_CPU),simulator)
else
$(error Unexpected PROJECT_CPU [$(PROJECT_CPU)])
endif

CPU_HWLAYER_DIR ?= $(PROJECT_DIR)/hw_layer/$(CPU_HWLAYER)

ifeq ($(CPU_STARTUP_DIR),)
CPU_STARTUP_DIR = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk/$(CPU_STARTUP)
endif

ifeq ($(GENERATED_ENUMS_DIR),)
GENERATED_ENUMS_DIR = $(PROJECT_DIR)/controllers/algo
endif
