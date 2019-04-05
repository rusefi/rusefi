##############################################################################
# Build global options
# NOTE: Can be overridden externally.
#

CHIBIOS = ChibiOS
RULESPATH = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC
RULESFILE = $(RULESPATH)/rules.mk

# Define project name here
PROJECT = rusefi
PROJECT_DIR = .

# Imported source files and paths
CHIBIOS_CONTRIB = ChibiOS-Contrib

include rusefi.mk

# by default EXTRA_PARAMS is empty and we create 'debug' version of the firmware with additional assertions and statistics
# for 'release' options see 'clean_compile_two_versions.bat' file

ifeq ($(DEBUG_LEVEL_OPT),)
  # this value would be used by default. For 'release' configuration override with '-O2' or something along these lines
  DEBUG_LEVEL_OPT = -O0 -ggdb -g3 
endif


# Compiler options here.
ifeq ($(USE_OPT),)
  USE_OPT = $(EXTRA_PARAMS) $(DEBUG_LEVEL_OPT) $(RFLAGS) -Wno-error=implicit-fallthrough -Wno-error=bool-operation -fomit-frame-pointer -falign-functions=16 -Werror-implicit-function-declaration -Werror -Wno-error=pointer-sign -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=sign-compare -Wno-error=unused-parameter -Wno-error=missing-field-initializers -Werror=type-limits -Wno-error=strict-aliasing -Wno-error=attributes
endif

# C specific options here (added to USE_OPT).
ifeq ($(USE_COPT),)
  USE_COPT = -fgnu89-inline -std=gnu99
endif

# C++ specific options here (added to USE_OPT).
ifeq ($(USE_CPPOPT),)
  # constexpr float expf_taylor_impl probably needs just c++14 but why not go with 17?
  USE_CPPOPT = -std=c++17 -Wno-register -fno-rtti -fno-threadsafe-statics -fno-exceptions -fno-use-cxa-atexit -Werror=write-strings -Werror=type-limits
endif

# Enable this if you want the linker to remove unused code and data
ifeq ($(USE_LINK_GC),)
  USE_LINK_GC = yes
endif

# Linker extra options here.
ifeq ($(USE_LDOPT),)
  USE_LDOPT =
endif

# Enable this if you want link time optimizations (LTO)
ifeq ($(USE_LTO),)
  USE_LTO = no
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

ifeq ($(USE_BOOTLOADER),)
  USE_BOOTLOADER = no
endif

ifeq ($(USE_FATFS),)
  USE_FATFS = yes
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
  USE_FPU = hard
endif

# and this is not working for be :( See https://github.com/rusefi/rusefi/issues/638
# use -j4 unless some value was specified
NUMJOBS=${NUMJOBS:-" -j4 "}

MAKEFLAGS += ${NUMJOBS}

#
# Architecture or project specific options
##############################################################################

##############################################################################
# Project, sources and paths
#

# Startup files.
include $(CPU_STARTUP_DIR)
# HAL-OSAL files (optional).
include $(CHIBIOS_CONTRIB)/os/hal/hal.mk
include $(CPU_PLATFORM_DIR)
include $(CHIBIOS)/os/hal/osal/rt/osal.mk
# RTOS files (optional).
include $(CHIBIOS)/os/rt/rt.mk
include $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/mk/port_v7m.mk

ifeq ($(USE_FATFS),yes)
include $(CHIBIOS)/os/various/fatfs_bindings/fatfs.mk
endif
include $(CHIBIOS)/os/various/cpp_wrappers/chcpp.mk

include console/binary/tunerstudio.mk
ifeq ($(USE_FATFS),yes)
include $(PROJECT_DIR)/ext/fatfs.mk
endif

include $(PROJECT_DIR)/hw_layer/hw_layer.mk
include $(PROJECT_DIR)/hw_layer/drivers/drivers.mk
include $(PROJECT_DIR)/hw_layer/sensors/sensors.mk
include $(PROJECT_DIR)/hw_layer/mass_storage/mass_storage.mk
include $(PROJECT_DIR)/development/development.mk
include $(PROJECT_DIR)/controllers/controllers.mk

include $(PROJECT_DIR)/util/util.mk
include $(PROJECT_DIR)/config/boards/$(PROJECT_BOARD)/board.mk
include $(PROJECT_DIR)/config/engines/engines.mk
include $(PROJECT_DIR)/controllers/algo/algo.mk
include $(PROJECT_DIR)/controllers/core/core.mk
include $(PROJECT_DIR)/controllers/math/math.mk
include $(PROJECT_DIR)/controllers/sensors/sensors.mk
include $(PROJECT_DIR)/controllers/system/system.mk
include $(PROJECT_DIR)/controllers/trigger/trigger.mk
include $(PROJECT_DIR)/console/console.mk

ifeq ($(BOOTLOADERINC),)
  # include default bootloader code
  BOOTLOADERINC= $(PROJECT_DIR)/bootloader/
endif


ifeq ($(USE_BOOTLOADER),yes)
include $(PROJECT_DIR)/bootloader/bootloader.mk
endif

# Define linker script file here
ifeq ($(LDSCRIPT),)
  LDSCRIPT= config/stm32f4ems/STM32F407xG.ld
endif

ifeq ($(PROJECT_CPU),)
  PROJECT_CPU=ST_STM32F4
endif                 

# C sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CSRC = $(STARTUPSRC) \
	$(KERNSRC) \
	$(PORTSRC) \
	$(OSALSRC) \
	$(HALSRC) \
	$(PLATFORMSRC) \
	$(BOARDSRC) \
	$(BOOTLOADERSRC) \
	$(CHIBIOS)/os/ex/ST/lis302dl.c \
	$(CHIBIOS)/os/various/syscalls.c \
	$(CHIBIOS)/os/hal/lib/streams/memstreams.c \
	$(CHIBIOS)/os/hal/lib/streams/chprintf.c \
	${HW_MASS_STORAGE_SRC_C} \
	$(UTILSRC) \
	$(ENGINES_SRC) \
	$(CONSOLESRC) \
	$(DEV_SRC) \
	$(HW_LAYER_EMS) \
	$(HW_LAYER_DRIVERS_CORE) \
	$(HW_LAYER_DRIVERS) \
	$(CONTROLLERSSRC) \
	$(CONTROLLERS_ALGO_SRC) \
	$(CONTROLLERS_CORE_SRC) \
	$(CONTROLLERS_SENSORS_SRC) \
	$(FATFSSRC) \
	$(SYSTEMSRC)

# C++ sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CPPSRC = $(CHCPPSRC) \
	$(TRIGGER_SRC_CPP) \
	$(TRIGGER_DECODERS_SRC_CPP) \
	$(DEV_SRC_CPP) \
	$(CONTROLLERS_ALGO_SRC_CPP) \
	$(SYSTEMSRC_CPP) \
	$(BOARDSRC_CPP) \
	$(ENGINES_SRC_CPP) \
	$(HW_LAYER_EMS_CPP) \
	$(HW_LAYER_DRIVERS_CPP) \
	$(HW_SENSORS_SRC) \
	$(TUNERSTUDIO_SRC_CPP) \
	$(CONSOLE_SRC_CPP) \
	$(CONTROLLERS_SENSORS_SRC_CPP) \
	$(CONTROLLERS_SRC_CPP) \
	$(UTILSRC_CPP) \
	$(CONTROLLERS_CORE_SRC_CPP) \
	$(CONTROLLERS_MATH_SRC_CPP) \
	rusefi.cpp \
	main.cpp

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

ifeq ($(CONFIGPATH),)
  CONFIGPATH=config/stm32f4ems
endif


INCDIR = $(CHIBIOS)/os/license \
        $(PORTINC) \
	$(OSALINC) \
	$(KERNINC) \
	$(TESTINC) \
	$(STARTUPINC) \
	$(HALINC) \
	$(HALINC_CONTRIB) \
	$(PLATFORMINC) \
	$(BOARDINC) \
	$(BOOTLOADERINC) \
	$(CHCPPINC) \
	$(CHIBIOS)/os/hal/lib/streams \
	$(CHIBIOS)/os/various \
	$(CHIBIOS)/os/ex/ST \
	$(CHIBIOS)/os/hal/lib/peripherals/sensors \
	$(CONFIGPATH) \
	config/engines \
	config \
	ext \
	ext_algo \
	console_util \
	console \
	$(PROJECT_DIR)/console/binary \
	$(PROJECT_DIR)/console/fl_binary \
	$(PROJECT_DIR)/hw_layer \
	$(PROJECT_DIR)/mass_storage \
	hw_layer/serial_over_usb \
	hw_layer/algo \
	hw_layer/lcd \
	$(HW_SENSORS_INC) \
	hw_layer/mass_storage \
	hw_layer/$(CPU_HWLAYER) \
	$(HW_LAYER_DRIVERS_INC) \
	$(UTIL_INC) \
	development \
	development/hw_layer \
	development/test \
	$(CONTROLLERS_INC) \
	controllers/sensors \
	controllers/system \
	controllers/algo \
	controllers/core \
	controllers/math \
	controllers/trigger/decoders \
	controllers/trigger

#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#

MCU  = cortex-m4

#TRGT = arm-elf-
TRGT = arm-none-eabi-
CC   = $(TRGT)gcc
CPPC = $(TRGT)g++
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
ULIBS = -lm

#
# End of user defines
##############################################################################

include $(RULESFILE)
