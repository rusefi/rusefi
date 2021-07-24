#
# this Makefile is intended to be invoked from one level above - see ..\compile_bootloader.sh
#

# by default EXTRA_PARAMS is empty and we create 'debug' version of the firmware with additional assertions and statistics
# for 'release' options see 'clean_compile_two_versions.bat' file

ifeq ($(DEBUG_LEVEL_OPT),)
  # this value would be used by default. For 'debug' configuration override with '-O0 -ggdb -g' or something along these lines
  DEBUG_LEVEL_OPT = -O2
  DDEFS += -DEFI_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_STACK_CHECK=FALSE -DCH_DBG_FILL_THREADS=FALSE -DCH_DBG_THREADS_PROFILING=FALSE
endif

# disable some modules to shrink bootloader binary
DDEFS += -DEFI_BOOTLOADER
DDEFS += -DHAL_USE_EXT=FALSE -DHAL_USE_ICU=FALSE -DHAL_USE_PWM=FALSE -DHAL_USE_RTC=FALSE -DEF_LUA=FALSE
#disable ChibiOS flsah driver and prevent header from include
DDEFS += -DHAL_USE_FLASH=FALSE

DDEFS += -DEFI_USE_UART_DMA=FALSE

# disable USB (The bootloader has currently UART support only)
DDEFS += -DEFI_USB_SERIAL=FALSE
# disable CAN
DDEFS += -DEFI_CAN_SUPPORT=FALSE

DDEFS += -DEFI_UNIT_TEST=0 -DEFI_PROD_CODE=1 -DEFI_SIMULATOR=0

# Compiler options here.
ifeq ($(USE_OPT),)
  USE_OPT = $(EXTRA_PARAMS) $(DEBUG_LEVEL_OPT) $(RFLAGS) -Wno-error=implicit-fallthrough -Wno-error=bool-operation -fomit-frame-pointer -falign-functions=16 -Werror -Wno-error=pointer-sign -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=sign-compare -Wno-error=unused-parameter -Wno-error=missing-field-initializers -Werror=type-limits -Wno-error=strict-aliasing -Wno-error=attributes
endif

# C specific options here (added to USE_OPT).
ifeq ($(USE_COPT),)
  USE_COPT = -fgnu89-inline -std=gnu99 -Werror-implicit-function-declaration
endif

# C++ specific options here (added to USE_OPT).
ifeq ($(USE_CPPOPT),)
  # constexpr float expf_taylor_impl probably needs just c++14 but why not go with 17?
  USE_CPPOPT = -std=c++17 -Wno-register -fno-rtti -fno-exceptions -fno-use-cxa-atexit -Werror=write-strings -Werror=type-limits -Winvalid-pch
endif

# Enable this if you want the linker to remove unused code and data
ifeq ($(USE_LINK_GC),)
  USE_LINK_GC = yes
endif

ifeq ($(CCACHE_DIR),)
 $(info No CCACHE_DIR)
else
 $(info CCACHE_DIR is ${CCACHE_DIR})
 CCPREFIX=ccache
endif

# Linker extra options here.
ifeq ($(USE_LDOPT),)
  USE_LDOPT =
endif

# Enable this if you want link time optimizations (LTO)
ifeq ($(USE_LTO),)
  USE_LTO = yes
endif

# If enabled, this option allows to compile the application in THUMB mode.
ifeq ($(USE_THUMB),)
  USE_THUMB = yes
endif

# Enable this if you want to see the full log while compiling.
ifeq ($(USE_VERBOSE_COMPILE),)
  USE_VERBOSE_COMPILE = no
endif

# If enabled, this option makes the build process faster by not compiling
# modules not used in the current configuration.
ifeq ($(USE_SMART_BUILD),)
  USE_SMART_BUILD = no
endif

#
# Build global options
##############################################################################

##############################################################################
# Architecture or project specific options
#

# Stack size to be allocated to the Cortex-M process stack. This stack is
# the stack used by the main() thread.
ifeq ($(USE_PROCESS_STACKSIZE),)
  USE_PROCESS_STACKSIZE = 0x0600
endif

# Stack size to the allocated to the Cortex-M main/exceptions stack. This
# stack is used for processing interrupts and exceptions.
ifeq ($(USE_EXCEPTIONS_STACKSIZE),)
  USE_EXCEPTIONS_STACKSIZE = 0x1000
endif

# Enables the use of FPU on Cortex-M4 (no, softfp, hard).
ifeq ($(USE_FPU),)
  USE_FPU = softfp
endif

#
# Architecture or project specific options
##############################################################################

##############################################################################
# Project, sources and paths
#

# Define project name here
PROJECT = bootloader
PROJECT_DIR = ..

# Configure precompiled header
PCH_DIR = $(PROJECT_DIR)/pch
PCHSRC = $(PCH_DIR)/pch.h

# Imported source files and paths
CHIBIOS = $(PROJECT_DIR)/ChibiOS
# todo: looks like 'CHIBIOS_CONTRIB' path is universal shall we defined it only once?
CHIBIOS_CONTRIB = $(CHIBIOS)/../ChibiOS-Contrib

RULESPATH = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk
RULESFILE = $(RULESPATH)/rules.mk

include $(PROJECT_DIR)/rusefi.mk

CONFIG = $(PROJECT_DIR)/config

# Include various ChibiOS mk files
# Licensing files.
include $(CHIBIOS)/os/license/license.mk
# Startup files.
include $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk/$(CPU_STARTUP)
# HAL-OSAL files (optional).
include $(CHIBIOS_CONTRIB)/os/hal/hal.mk
include $(CPU_PLATFORM)
include $(CHIBIOS)/os/hal/osal/rt-nil/osal.mk
# RTOS files (optional).
include $(CHIBIOS)/os/rt/rt.mk
include $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/mk/port_v7m.mk
include $(CHIBIOS)/os/various/cpp_wrappers/chcpp.mk
# EX files (optional).
include $(CHIBIOS)/os/hal/lib/streams/streams.mk

include $(CONFIG)/boards/$(PROJECT_BOARD)/board.mk
include $(PROJECT_DIR)/init/init.mk
include $(PROJECT_DIR)/util/util.mk

include $(PROJECT_DIR)/controllers/controllers.mk
include $(PROJECT_DIR)/hw_layer/$(CPU_HWLAYER)/hw_ports.mk
include $(PROJECT_DIR)/hw_layer/drivers/drivers.mk

# C sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CSRC =  $(ALLCSRC) \
	$(CHIBIOS)/os/various/syscalls.c \
	$(CONSOLESRC) \
	$(DEV_SRC) \
	$(HW_LAYER_EMS) \
	$(HW_LAYER_DRIVERS_CORE) \
	$(FATFSSRC) \
	$(SYSTEMSRC) \

# C++ sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CPPSRC = $(ALLCPPSRC) \
	$(DEV_SRC_CPP) \
	$(HW_LAYER_EMS_CPP) \
	$(HW_SENSORS_SRC) \
	$(TUNERSTUDIO_SRC_CPP) \
	$(CONSOLE_SRC_CPP) \
	$(PROJECT_DIR)/console/binary/tunerstudio_io.cpp \
	$(PROJECT_DIR)/console/binary/tunerstudio_io_serial.cpp \
	$(PROJECT_DIR)/controllers/system/efi_gpio.cpp \
	$(PROJECT_DIR)/controllers/algo/engine_configuration.cpp \
	$(PROJECT_DIR)/controllers/persistent_store.cpp \
	$(PROJECT_DIR)/hw_layer/io_pins.cpp \
	$(PROJECT_DIR)/util/efilib.cpp \
	$(PROJECT_DIR)/hw_layer/pin_repository.cpp \
	src/rusefi_stubs.cpp \
	src/dfu.cpp \
	src/main.cpp

# C sources to be compiled in ARM mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
ACSRC =

# C++ sources to be compiled in ARM mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
ACPPSRC =

# C sources to be compiled in THUMB mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
TCSRC =

# C sources to be compiled in THUMB mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
TCPPSRC =

# List ASM source files here
# List ASM source files here
ASMXSRC = $(STARTUPASM) $(PORTASM) $(OSALASM)

INCDIR = $(ALLINC) \
	$(PCH_DIR) \
	.. \
	$(CHIBIOS)/os/various \
	$(CHIBIOS)/os/ex/ST \
	$(CHIBIOS)/os/hal/lib/peripherals/sensors \
	$(CONFIG)/engines \
	$(CONFIG) \
	$(CONFDIR) \
	$(PROJECT_DIR)/ext \
	$(PROJECT_DIR)/ext_algo \
	$(UTIL_INC) \
	$(PROJECT_DIR)/console_util \
	$(PROJECT_DIR)/console \
	$(PROJECT_DIR)/console/binary \
	$(PROJECT_DIR)/console/binary_log \
	$(PROJECT_DIR)/console/fl_binary \
	$(PROJECT_DIR)/hw_layer \
	$(PROJECT_DIR)/hw_layer/adc \
	$(PROJECT_DIR)/hw_layer/mass_storage \
	$(PROJECT_DIR)/hw_layer/serial_over_usb \
	$(PROJECT_DIR)/hw_layer/algo \
	$(PROJECT_DIR)/hw_layer/lcd \
	$(PROJECT_DIR)/hw_layer/sensors \
	$(PROJECT_DIR)/hw_layer/mass_storage \
	$(PROJECT_DIR)/hw_layer/ports \
	$(PROJECT_DIR)/hw_layer/$(CPU_HWLAYER) \
	$(HW_INC) \
	$(HW_LAYER_DRIVERS_INC) \
	$(PROJECT_DIR)/development \
	$(PROJECT_DIR)/development/hw_layer \
	$(PROJECT_DIR)/development/test \
	$(CONTROLLERS_INC) \
	$(PROJECT_DIR)/controllers/sensors \
	$(PROJECT_DIR)/init \
	config

BUILDDIR=blbuild

#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#

MCU  = cortex-m4

ifeq ($(CROSS_COMPILE),)
  #TRGT = arm-elf-
  TRGT = arm-none-eabi-
else
  TRGT = $(CROSS_COMPILE)
endif
CC   = $(CCPREFIX) $(TRGT)gcc
CPPC = $(CCPREFIX) $(TRGT)g++
# Enable loading with g++ only if you need C++ runtime support.
# NOTE: You can use C++ even without C++ support if you are careful. C++
#       runtime support makes code size explode.
LD   = $(TRGT)gcc
#LD   = $(TRGT)g++
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
AR   = $(TRGT)ar
OD   = $(TRGT)objdump
SZ   = $(TRGT)size
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary

# ARM-specific options here
AOPT =

# THUMB-specific options here
TOPT = -mthumb -DTHUMB

# Define C warning options here
CWARN = -Wall -Wextra -Wstrict-prototypes

# Define C++ warning options here
CPPWARN = -Wall -Wextra

#
# Compiler settings
##############################################################################

##############################################################################
# Start of user section
#

# List all user C define here, like -D_DEBUG=1
UDEFS =

# Define ASM defines here
UADEFS =

# List all user directories here
UINCDIR =

# List the user directory to look for the libraries here
ULIBDIR =

# List all user libraries here
ULIBS = -lm --specs=nano.specs

#
# End of user defines
##############################################################################

include $(RULESFILE)

# TODO: why does PCH not behave itself in bootloader?
include $(PROJECT_DIR)/rusefi_pch.mk
