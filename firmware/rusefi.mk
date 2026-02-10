
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

# Build the generated pin code only if the connector directory exists
ifneq ("$(wildcard $(BOARD_DIR)/connectors)","")
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
else ifeq ($(PROJECT_CPU),custom_platform)
  include $(BOARD_DIR)/custom_platform.mk
  $(info Using custom CPU_STARTUP_DIR $(CPU_STARTUP_DIR))
  $(info Using custom CPU_PLATFORM $(CPU_PLATFORM))
  $(info Using custom CPU_HWLAYER $(CPU_HWLAYER))
else ifeq ($(PROJECT_CPU),simulator)
else
$(error Unexpected PROJECT_CPU [$(PROJECT_CPU)])
endif

ifeq ($(CPU_STARTUP_DIR),)
CPU_STARTUP_DIR = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk/$(CPU_STARTUP)
endif

ifeq ($(GENERATED_ENUMS_DIR),)
GENERATED_ENUMS_DIR = $(PROJECT_DIR)/controllers/algo
endif
