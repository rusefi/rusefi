##############################################################################
# Build global options
# NOTE: Can be overridden externally.
#

#
# As of Sep 2019, this is known to compile on Linux
# in my case I had to
# sudo apt-get install gcc-multilib g++-multilib
# 
# todo: automatically execute that command same as we automatically fetch submodules?
# 



CHIBIOS = ../firmware/ChibiOS
CHIBIOS_CONTRIB= ../firmware/ChibiOS-Contrib
RULESPATH = $(CHIBIOS)/os/common/startup/SIMIA32/compilers/GCC
RULESFILE = $(RULESPATH)/rules.mk

PROJECT_CPU=simulator

include ../firmware/rusefi.mk
RULESFILE = ../firmware/rusefi_rules.mk

# Configure precompiled header
PCH_DIR = $(PROJECT_DIR)/pch
PCHSRC = $(PCH_DIR)/pch.h
PCHSUB = simulator

# used by USE_SMART_BUILD
CONFDIR = .


# Compiler options here.
ifeq ($(USE_OPT),)
# this config if debugging is needed, but the binary is about 50M
#  USE_OPT = -Wall     -ggdb    -Werror-implicit-function-declaration -Werror -Wno-error=pointer-sign -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=sign-compare -Wno-error=unused-parameter -Wno-error=missing-field-initializers -Wno-error=write-strings -Wno-error=strict-aliasing
#  USE_OPT = -Wall -O0 -ggdb -g -Wno-error=implicit-fallthrough -Werror-implicit-function-declaration -Wno-error=pointer-sign -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=sign-compare -Wno-error=unused-parameter -Wno-error=missing-field-initializers -Wno-error=write-strings
# this config producec a smaller binary file
# 7.3 compiler would want -Wno-error=implicit-fallthrough while 6.4 does not know about it
# see https://github.com/rusefi/rusefi/issues/517
  USE_OPT = -Wall -O2 -Wno-error=implicit-fallthrough -Wno-error=write-strings -Wno-error=strict-aliasing

	ifeq ($(OS),Windows_NT)
	else
 	 USE_OPT +=  -m32
	endif
endif

# See explanation in main firmware Makefile for these three defines
USE_OPT += $(RUSEFI_OPT) -DEFI_UNIT_TEST=0 -DEFI_PROD_CODE=0 -DEFI_SIMULATOR=1

ifeq ($(CCACHE_DIR),)
 $(info No CCACHE_DIR)
else
 $(info CCACHE_DIR is ${CCACHE_DIR})
 CCPREFIX=ccache
endif

# C specific options here (added to USE_OPT).
ifeq ($(USE_COPT),)
  USE_COPT = -std=gnu99
endif

# C++ specific options here (added to USE_OPT).
ifeq ($(USE_CPPOPT),)
  USE_CPPOPT = -std=c++17 -Wno-register -fno-rtti
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
  USE_SMART_BUILD = no
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
ifeq ($(OS),Windows_NT)
  PROJECT = rusefi_simulator.exe
else
  PROJECT = rusefi_simulator
endif
PROJECT_DIR = ../firmware

# Imported source files and paths

# Licensing files.
include $(CHIBIOS)/os/license/license.mk
# Startup files.
# HAL-OSAL files (optional).
include $(CHIBIOS)/os/hal/hal.mk
include $(CHIBIOS)/os/hal/boards/simulator/board.mk
include $(CHIBIOS)/os/hal/osal/rt-nil/osal.mk
# RTOS files (optional).
include $(CHIBIOS)/os/rt/rt.mk
include $(CHIBIOS)/os/common/ports/SIMIA32/compilers/GCC/port.mk
# Other files (optional).
include $(CHIBIOS)/os/hal/lib/streams/streams.mk
#include $(CHIBIOS)/os/various/cpp_wrappers/chcpp.mk

ifeq ($(OS),Windows_NT)
  include ${CHIBIOS}/os/hal/ports/simulator/win32/platform.mk
else
  include ${CHIBIOS}/os/hal/ports/simulator/posix/platform.mk
endif

include $(PROJECT_DIR)/console/binary/tunerstudio.mk
include $(PROJECT_DIR)/console/console.mk
include $(PROJECT_DIR)/config/engines/engines.mk
include $(PROJECT_DIR)/controllers/controllers.mk
include $(PROJECT_DIR)/controllers/lua/lua.mk
include $(PROJECT_DIR)/development/development.mk
include $(PROJECT_DIR)/hw_layer/hw_layer.mk
include $(PROJECT_DIR)/hw_layer/drivers/drivers.mk
include $(PROJECT_DIR)/hw_layer/sensors/sensors.mk
include $(PROJECT_DIR)/init/init.mk
include $(PROJECT_DIR)/util/util.mk


# C sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CSRC =  $(ALLCSRC) \
        $(SYSTEMSRC) \
        $(CONSOLESRC) \
        $(CONTROLLERS_ALGO_SRC) \
        $(CONTROLLERS_CORE_SRC) \
        $(CONTROLLERS_SENSORS_SRC) \
        $(ENGINES_SRC) \
        $(UTILSRC) \
        main.c

# C++ sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CPPSRC = $(ALLCPPSRC) \
  $(CHIBIOS)/os/various/cpp_wrappers/ch.cpp \
        $(UTILSRC_CPP) \
	$(CONTROLLERS_ALGO_SRC_CPP) \
	$(CONTROLLERS_SRC_CPP) \
	$(PROJECT_DIR)/development/sensor_chart.cpp \
	$(HW_LAYER_EMS_CPP) \
	$(HW_SENSORS_CPP) \
	$(HW_LAYER_DRIVERS_CPP) \
	$(HW_LAYER_DRIVERS_CORE_CPP) \
	$(TRIGGER_SRC_CPP) \
	$(TRIGGER_DECODERS_SRC_CPP) \
	$(SYSTEMSRC_CPP) \
	$(TUNERSTUDIO_SRC_CPP) \
	$(CONSOLE_SRC_CPP) \
	$(INIT_SRC_CPP) \
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
          $(PCH_DIR) \
          $(ALLINC) \
          $(CHIBIOS)/os/various/cpp_wrappers \
          $(UTIL_INC) \
          $(PROJECT_DIR)/init \
          $(CONSOLE_INC) \
          $(PROJECT_DIR)/config/engines \
		  $(PROJECT_DIR)/config/boards/ \
          $(PROJECT_DIR)/ext_algo \
          $(HW_LAYER_DRIVERS_INC) \
          $(HW_LAYER_INC) \
	  $(HW_SENSORS_INC) \
          $(PROJECT_DIR)/hw_layer/algo \
          $(PROJECT_DIR)/hw_layer/drivers/can \
          $(HW_SENSORS_INC) \
          $(DEVELOPMENT_DIR) \
          $(CONTROLLERS_INC) \
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

$(info OS is [${OS}])

ifeq ($(OS),Windows_NT)
  # ChibiOS seem to require 32 bit compiler at least on Windows
  # base 32-bit Cygwin is needed with mingw64 32-bit version 
  # Cygwin64 would not debug if used with mingw64 32-bit version
  # 32 bit GDB available at http://www.equation.com/servlet/equation.cmd?fa=gdb

  TRGT = i686-w64-mingw32-
else
  TRGT = 
endif

CC   = $(CCPREFIX) $(TRGT)gcc
CPPC = $(CCPREFIX) $(TRGT)g++
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

include $(RULESPATH)/rules.mk

# Enable precompiled header
include $(PROJECT_DIR)/rusefi_pch.mk
