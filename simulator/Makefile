##############################################################################
# Build global options
# NOTE: Can be overridden externally.
#

# Compiler options here.
ifeq ($(USE_OPT),)
# this config if debugging is needed, but the binary is about 50M
#  USE_OPT = -Wall     -ggdb     -Werror-implicit-function-declaration -Werror -Wno-error=pointer-sign -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=sign-compare -Wno-error=unused-parameter -Wno-error=missing-field-initializers -Wno-error=write-strings -Wno-error=strict-aliasing
#  USE_OPT = -Wall -O0 -ggdb -g3 -Werror-implicit-function-declaration -Werror -Wno-error=pointer-sign -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=sign-compare -Wno-error=unused-parameter -Wno-error=missing-field-initializers -Wno-error=write-strings
# this config producec a smaller binary file
  USE_OPT = -Wall -O2           -Werror-implicit-function-declaration -Werror -Wno-error=pointer-sign -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=sign-compare -Wno-error=unused-parameter -Wno-error=missing-field-initializers -Wno-error=write-strings -Wno-error=strict-aliasing

	ifeq ($(OS),Windows_NT)
	else
 	 USE_OPT +=  -m32
	endif
endif

# C specific options here (added to USE_OPT).
ifeq ($(USE_COPT),)
  USE_COPT = -std=gnu99
endif

# C++ specific options here (added to USE_OPT).
ifeq ($(USE_CPPOPT),)
  USE_CPPOPT = -fno-rtti
endif

# Enable this if you want the linker to remove unused code and data.
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

# Enable this if you want to see the full log while compiling.
ifeq ($(USE_VERBOSE_COMPILE),)
  USE_VERBOSE_COMPILE = no
endif

# If enabled, this option makes the build process faster by not compiling
# modules not used in the current configuration.
ifeq ($(USE_SMART_BUILD),)
  USE_SMART_BUILD = yes
endif

#
# Build global options
##############################################################################

##############################################################################
# Architecture or project specific options
#

#
# Architecture or project specific options
##############################################################################

##############################################################################
# Project, sources and paths
#

# Define project name here
PROJECT = rusefi_simulator
PROJECT_DIR = ../firmware

# Imported source files and paths
CHIBIOS = ../firmware/ChibiOS4

# Startup files.
# HAL-OSAL files (optional).
include $(CHIBIOS)/os/hal/hal.mk
include $(CHIBIOS)/os/hal/boards/simulator/board.mk
include $(CHIBIOS)/os/hal/osal/rt/osal.mk
# RTOS files (optional).
include $(CHIBIOS)/os/rt/rt.mk
include $(CHIBIOS)/os/common/ports/SIMIA32/compilers/GCC/port.mk
# Other files (optional).

ifeq ($(OS),Windows_NT)
  include ${CHIBIOS}/os/hal/ports/simulator/win32/platform.mk
else
  include ${CHIBIOS}/os/hal/ports/simulator/posix/platform.mk
endif

include $(PROJECT_DIR)/util/util.mk
include $(PROJECT_DIR)/config/engines/engines.mk
include $(PROJECT_DIR)/controllers/algo/algo.mk
include $(PROJECT_DIR)/controllers/core/core.mk
include $(PROJECT_DIR)/controllers/math/math.mk
include $(PROJECT_DIR)/controllers/sensors/sensors.mk
include $(PROJECT_DIR)/controllers/trigger/trigger.mk
include $(PROJECT_DIR)/controllers/system/system.mk
include $(PROJECT_DIR)/console/console.mk
include $(PROJECT_DIR)/console/binary/tunerstudio.mk
include $(PROJECT_DIR)/development/development.mk
include $(PROJECT_DIR)/hw_layer/hw_layer.mk


# C sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CSRC =  ${PORTSRC} \
        ${KERNSRC} \
        ${HALSRC} \
        ${PLATFORMSRC} \
        $(SYSTEMSRC) \
        $(CONSOLESRC) \
        $(CONTROLLERS_ALGO_SRC) \
        $(CONTROLLERS_CORE_SRC) \
        $(CONTROLLERS_SENSORS_SRC) \
        $(ENGINES_SRC) \
        $(BOARDSRC) \
        $(CHIBIOS)/os/hal/lib/streams/memstreams.c \
        $(CHIBIOS)/os/hal/lib/streams/chprintf.c \
        $(UTILSRC) \
        main.c

# todo: reuse controllers.mk here 
# C++ sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CPPSRC = $(UTILSRC_CPP) \
	$(CONTROLLERS_ALGO_SRC_CPP) \
	$(PROJECT_DIR)/controllers/settings.cpp \
	$(PROJECT_DIR)/controllers/engine_controller.cpp \
	$(PROJECT_DIR)/controllers/error_handling.cpp \
	$(PROJECT_DIR)/controllers/persistent_store.cpp \
	$(PROJECT_DIR)/controllers/map_averaging.cpp \
	$(PROJECT_DIR)/development/sensor_chart.cpp \
	$(PROJECT_DIR)/development/trigger_emulator.cpp \
	$(TRIGGER_SRC_CPP) \
	$(TRIGGER_DECODERS_SRC_CPP) \
	$(SYSTEMSRC_CPP) \
	$(TUNERSTUDIO_SRC_CPP) \
	$(CONSOLE_SRC_CPP) \
	$(CONTROLLERS_SENSORS_SRC_CPP) \
	$(CONTROLLERS_CORE_SRC_CPP) \
	$(CONTROLLERS_MATH_SRC_CPP) \
	$(DEV_SIMULATOR_SRC_CPP) \
	$(ENGINES_SRC_CPP) \
	simulator/rusEfiFunctionalTest.cpp \
	simulator/framework.cpp \
	simulator/boards.cpp \
	$(TEST_SRC_CPP)


# List ASM source files here
ASMSRC = $(PORTASM)

INCDIR = . \
          $(CHIBIOS)/os/license \
          $(PORTINC) \
          $(KERNINC) \
          $(OSALINC) \
          $(HALINC) \
          $(PLATFORMINC) \
          $(BOARDINC) \
          $(PROJECT_DIR)/util \
          $(PROJECT_DIR)/console \
          $(PROJECT_DIR)/console/binary \
          $(PROJECT_DIR)/console/fl_binary \
          $(PROJECT_DIR)/config/engines \
          $(PROJECT_DIR)/ext_algo \
          $(PROJECT_DIR)/controllers \
          $(PROJECT_DIR)/hw_layer \
          $(PROJECT_DIR)/hw_layer/algo \
          $(PROJECT_DIR)/development \
          $(PROJECT_DIR)/controllers/algo \
          $(PROJECT_DIR)/controllers/core \
          $(PROJECT_DIR)/controllers/math \
          $(PROJECT_DIR)/controllers/sensors \
          $(PROJECT_DIR)/controllers/system \
          $(PROJECT_DIR)/controllers/trigger \
          $(PROJECT_DIR)/controllers/trigger/decoders \
          ${CHIBIOS}/os/various \
          $(CHIBIOS)/os/hal/lib/streams \
          simulator

# List ASM source files here
ASMSRC =
ASMXSRC = $(STARTUPASM) $(PORTASM) $(OSALASM)

#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#

ifeq ($(OS),Windows_NT)
  # base 32-bit Cygwin is needed with mingw64 32-bit version 
  # Cygwin64 would not debug if used with mingw64 32-bit version

  TRGT = i686-w64-mingw32-
else
  TRGT = 
endif

CC   = $(TRGT)gcc
CPPC = $(TRGT)g++
# Enable loading with g++ only if you need C++ runtime support.
# NOTE: You can use C++ even without C++ support if you are careful. C++
#       runtime support makes code size explode.
#LD   = $(TRGT)gcc
LD   = $(TRGT)g++
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
AR   = $(TRGT)ar
OD   = $(TRGT)objdump
SZ   = $(TRGT)size
BIN  = $(CP) -O binary
COV  = gcov

# Define C warning options here
CWARN = -Wall -Wextra -Wundef -Wstrict-prototypes

# Define C++ warning options here
CPPWARN = -Wall -Wextra -Wundef

#
# Compiler settings
##############################################################################

##############################################################################
# Start of user section
#

# List all user C define here, like -D_DEBUG=1
UDEFS = -DSIMULATOR

# Define ASM defines here
UADEFS =

# List all user directories here
UINCDIR =

# List the user directory to look for the libraries here
ULIBDIR =

# List all user libraries here
ifeq ($(OS),Windows_NT)
ULIBS = -lws2_32 -static
else
ULIBS =
endif

#
# End of user defines
##############################################################################

RULESPATH = $(CHIBIOS)/os/common/startup/SIMIA32/compilers/GCC
include $(RULESPATH)/rules.mk
