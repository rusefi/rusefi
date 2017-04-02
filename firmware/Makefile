##############################################################################
# Build global options
# NOTE: Can be overridden externally.
#

PROJECT_DIR = .

# by default EXTRA_PARAMS is empty and we create 'debug' version of the firmware with additional assertions and statistics
# for 'release' options see 'clean_compile_two_versions.bat' file


# Compiler options here.
ifeq ($(USE_OPT),)
#  USE_OPT = -O2 -ggdb -std=gnu99 -fomit-frame-pointer -falign-functions=16
#  USE_OPT = $(RFLAGS) -O1 -fgnu89-inline -ggdb -fomit-frame-pointer -falign-functions=16 -std=gnu99 -Werror-implicit-function-declaration -Werror -Wno-error=pointer-sign -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=sign-compare -Wno-error=unused-parameter -Wno-error=missing-field-initializers
  USE_OPT = $(EXTRA_PARAMS) $(RFLAGS) -O2        -fomit-frame-pointer -falign-functions=16 -Werror-implicit-function-declaration -Werror -Wno-error=pointer-sign -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=sign-compare -Wno-error=unused-parameter -Wno-error=missing-field-initializers -Werror=type-limits -Wno-error=strict-aliasing -Wno-error=attributes
endif

# C specific options here (added to USE_OPT).
ifeq ($(USE_COPT),)
  USE_COPT = -fgnu89-inline -std=gnu99
endif

# C++ specific options here (added to USE_OPT).
ifeq ($(USE_CPPOPT),)
  USE_CPPOPT = -std=c++11 -fno-rtti -fno-exceptions -fno-use-cxa-atexit -Werror=write-strings -Werror=type-limits
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

#
# Build global options
##############################################################################

##############################################################################
# Architecture or project specific options
#

USE_FPU = softfp

# Enables the use of FPU on Cortex-M4 (no, softfp, hard).
ifeq ($(USE_FPU),)
  USE_FPU = no
endif

#
# Architecture or project specific options
##############################################################################

##############################################################################
# Project, sources and paths
#

# Define project name here
PROJECT = rusefi

# Imported source files and paths
CHIBIOS = chibios
#include $(CHIBIOS)/test/test.mk

#PROJECT_BOARD = OLIMEX_STM32_E407
ifneq ($(PROJECT_BOARD),OLIMEX_STM32_E407)
  PROJECT_BOARD = ST_STM32F4
endif
DDEFS += -D$(PROJECT_BOARD)


# Imported source files and paths
include $(CHIBIOS)/os/hal/platforms/STM32F4xx/platform.mk
include $(CHIBIOS)/os/hal/hal.mk
include $(CHIBIOS)/os/ports/GCC/ARMCMx/STM32F4xx/port.mk
include $(CHIBIOS)/os/kernel/kernel.mk
#include $(CHIBIOS)/os/various/cpp_wrappers/kernel.mk

include $(CHIBIOS)/os/various/fatfs_bindings/fatfs.mk
include console/binary/tunerstudio.mk
include ext/ext.mk
include $(PROJECT_DIR)/hw_layer/hw_layer.mk
include $(PROJECT_DIR)/hw_layer/sensors/sensors.mk
include $(PROJECT_DIR)/hw_layer/mass_storage/mass_storage.mk
include development/development.mk
include controllers/controllers.mk

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


# Define linker script file here
LDSCRIPT= config/stm32f4ems/STM32F407xG_CCM.ld

# C sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CSRC = $(PORTSRC) \
	$(KERNSRC) \
	chibios/os/various/syscalls.c \
	chibios/os/various/chprintf.c \
	chibios/os/various/memstreams.c \
	chibios/os/various/chrtclib.c  \
	${HW_MASS_STORAGE_SRC_C} \
	$(UTILSRC) \
	$(ENGINES_SRC) \
	$(CONSOLESRC) \
	$(HALSRC) \
	$(DEV_SRC) \
	$(HW_LAYER_EMS) \
	$(CONTROLLERSSRC) \
	$(CONTROLLERS_ALGO_SRC) \
	$(CONTROLLERS_CORE_SRC) \
	$(CONTROLLERS_SENSORS_SRC) \
	$(PLATFORMSRC) \
	$(BOARDSRC) \
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
	$(ENGINES_SRC_CPP) \
	$(HW_LAYER_EMS_CPP) \
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
ASMSRC = $(PORTASM)

INCDIR = $(PORTINC) $(KERNINC) $(TESTINC) \
         $(HALINC) $(PLATFORMINC) $(BOARDINC) \
         $(CHCPPINC) \
         $(CHIBIOS)/os/various \
         $(CHIBIOS)/os/various/devices_lib/accel \
	config/stm32f4ems \
	config/engines \
	config \
	chibios/os/various \
	ext \
	ext_algo \
	util \
	console_util \
	console \
	$(PROJECT_DIR)/console/binary \
	$(PROJECT_DIR)/console/fl_binary \
	$(PROJECT_DIR)/hw_layer \
	$(PROJECT_DIR)/mass_storage \
	hw_layer/serial_over_usb \
	hw_layer/algo \
	hw_layer/lcd \
	hw_layer/sensors \
	hw_layer/mass_storage \
	hw_layer/stm32f4 \
	development \
	development/hw_layer \
	development/test \
	controllers \
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
# Start of default section
#

# List all default C defines here, like -D_DEBUG=1
DDEFS =

# List all default ASM defines here, like -D_DEBUG=1
DADEFS =

# List all default directories to look for include files here
DINCDIR =

# List the default directory to look for the libraries here
DLIBDIR =

# List all default libraries here
DLIBS =

#
# End of default section
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

RULESPATH = $(CHIBIOS)/os/ports/GCC/ARMCMx
include $(RULESPATH)/rules.mk
