
ifeq ("$(wildcard $(RULESFILE))","")
$(info Invoking "git submodule update --init")
$(shell git submodule update --init)
$(info Invoked "git submodule update --init")
# make is not happy about newly checked out module for some reason but next invocation would work
$(error Please run 'make' again)
endif

ifeq ($(PROJECT_BOARD),)
  PROJECT_BOARD = ST_STM32F4
endif
DDEFS += -D$(PROJECT_BOARD)

ifeq ($(PROJECT_CPU),)
  PROJECT_CPU = ST_STM32F4
endif
DDEFS += -D$(PROJECT_CPU)

# CPU-dependent defs
ifeq ($(PROJECT_CPU),ST_STM32F7)
CPU_STARTUP = startup_stm32f7xx.mk
CPU_PLATFORM = STM32F7xx/platform.mk
CPU_HWLAYER = stm32f7
endif

ifeq ($(PROJECT_CPU),ST_STM32F4)
CPU_STARTUP = startup_stm32f4xx.mk
CPU_PLATFORM = STM32F4xx/platform.mk
CPU_HWLAYER = stm32f4
endif


ifeq ($(PROJECT_CPU),ST_STM32F1)
CPU_STARTUP = startup_stm32f1xx.mk
CPU_PLATFORM = STM32F1xx/platform.mk
CPU_HWLAYER = stm32f1
endif
