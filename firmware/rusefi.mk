
ifeq ("$(wildcard $(RULESFILE))","")
$(info $(RULESFILE) not found. Chibios: Invoking "git submodule update --init")
$(shell git submodule update --init)
$(info Invoked "git submodule update --init")
# make is not happy about newly checked out module for some reason but next invocation would work
$(error Please run 'make' again. Please make sure you have 'git' command in PATH)
endif

CHIBIOS_CONTRIB_FILE=$(CHIBIOS_CONTRIB)/os/hal/hal.mk
ifeq ("$(wildcard $(CHIBIOS_CONTRIB_FILE))","")
$(info $(CHIBIOS_CONTRIB_FILE) not found. Contrib: Invoking "git submodule update --init")
$(shell git submodule update --init)
$(info Invoked "git submodule update --init")
# make is not happy about newly checked out module for some reason but next invocation would work
$(error Please run 'make' again. Please make sure you have 'git' command in PATH)
endif

ifeq ($(PROJECT_BOARD),)
  PROJECT_BOARD = f407-discovery
endif

ifeq ($(PROJECT_CPU),)
  # many boards all the way to Proteus use this F4 default
  PROJECT_CPU = ARCH_STM32F4
endif

-include $(PROJECT_DIR)/config/boards/$(PROJECT_BOARD)/config.mk

PIN_NAMES_FILE=$(PROJECT_DIR)/config/boards/$(PROJECT_BOARD)/connectors/generated_ts_name_by_pin.cpp

ifneq ("$(wildcard $(PIN_NAMES_FILE))","")
$(info found $(PIN_NAMES_FILE) )
ALLCPPSRC += $(PIN_NAMES_FILE)
endif

# CPU-dependent defs
ifeq ($(PROJECT_CPU),ARCH_STM32F7)
	CPU_STARTUP = startup_stm32f7xx.mk
	# next file is included through Contrib's platform.ml
	#CPU_PLATFORM = $(CHIBIOS)/os/hal/ports/STM32/STM32F7xx/platform.mk
	CPU_PLATFORM = ${CHIBIOS_CONTRIB}/os/hal/ports/STM32/STM32F7xx/platform.mk
	CPU_HWLAYER = ports/stm32/stm32f7
else ifeq ($(PROJECT_CPU),ARCH_STM32F4)
	CPU_STARTUP = startup_stm32f4xx.mk
	# next file is included through Contrib's platform.ml
	#CPU_PLATFORM = $(CHIBIOS)/os/hal/ports/STM32/STM32F4xx/platform.mk
	CPU_PLATFORM = ${CHIBIOS_CONTRIB}/os/hal/ports/STM32/STM32F4xx/platform.mk
	CPU_HWLAYER = ports/stm32/stm32f4
else ifeq ($(PROJECT_CPU),ARCH_STM32H7)
	CPU_STARTUP = startup_stm32h7xx.mk
	# next file is included through Contrib's platform.ml
	#CPU_PLATFORM = $(CHIBIOS)/os/hal/ports/STM32/STM32H7xx/platform.mk
	CPU_PLATFORM = ${CHIBIOS_CONTRIB}/os/hal/ports/STM32/STM32H7xx/platform.mk
	CPU_HWLAYER = ports/stm32/stm32h7
else ifeq ($(PROJECT_CPU),kinetis)
	CPU_STARTUP_DIR = $(KINETIS_CONTRIB)/os/common/startup/ARMCMx/compilers/GCC/mk/startup_ke1xf.mk
	CPU_PLATFORM = $(KINETIS_CONTRIB)/os/hal/ports/KINETIS/KE1xF/platform.mk
	CPU_HWLAYER = ports/kinetis
else ifeq ($(PROJECT_CPU),cypress)
	CPU_STARTUP_DIR = $(CYPRESS_CONTRIB)/os/common/startup/ARMCMx/compilers/GCC/mk/startup_S6E2CxAH.mk
	CPU_PLATFORM = $(CYPRESS_CONTRIB)/os/hal/ports/Cypress/S6E2CxAH/platform.mk
	CPU_HWLAYER = ports/cypress
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
