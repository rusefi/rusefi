
ifeq ("$(wildcard $(RULESFILE))","")
$(info Invoking "git submodule update --init")
$(shell git submodule update --init)
$(info Invoked "git submodule update --init")
# make is not happy about newly checked out module for some reason but next invocation would work
$(error Please run 'make' again)
endif

ifeq ($(PROJECT_BOARD),)
  PROJECT_BOARD = st_stm32f4
endif

ifeq ($(PROJECT_CPU),)
  PROJECT_CPU = ARCH_STM32F4
endif

-include $(PROJECT_DIR)/config/boards/$(PROJECT_BOARD)/config.mk

# CPU-dependent defs
ifeq ($(PROJECT_CPU),ARCH_STM32F7)
CPU_STARTUP = startup_stm32f7xx.mk
CPU_PLATFORM = STM32F7xx/platform.mk
CPU_HWLAYER = ports/stm32/stm32f7
endif

ifeq ($(PROJECT_CPU),ARCH_STM32F4)
CPU_STARTUP = startup_stm32f4xx.mk
CPU_PLATFORM = STM32F4xx/platform.mk
CPU_HWLAYER = ports/stm32/stm32f4
endif


ifeq ($(PROJECT_CPU),ARCH_STM32F1)
CPU_STARTUP = startup_stm32f1xx.mk
CPU_PLATFORM = STM32F1xx/platform.mk
CPU_HWLAYER = ports/stm32/stm32f1
endif

ifeq ($(CPU_STARTUP_DIR),)
CPU_STARTUP_DIR = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk/$(CPU_STARTUP)
endif

ifeq ($(CPU_PLATFORM_DIR),)
CPU_PLATFORM_DIR = $(CHIBIOS)/os/hal/ports/STM32/$(CPU_PLATFORM)
endif

ifeq ($(GENERATED_ENUMS_DIR),)
GENERATED_ENUMS_DIR = $(PROJECT_DIR)/controllers/algo
endif
