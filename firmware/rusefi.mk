
ifeq ("$(wildcard $(RULESFILE))","")
#
# todo: someone please figure out the syntax to invoke 'git submodule update --init' from here
#
$(error Modules are missing? Did you execute 'git submodule update --init'?)
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
else # ST_STM32F4
CPU_STARTUP = startup_stm32f4xx.mk
CPU_PLATFORM = STM32F4xx/platform.mk
CPU_HWLAYER = stm32f4
endif
