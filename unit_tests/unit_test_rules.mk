UNIT_TESTS_DIR=$(PROJECT_DIR)/../unit_tests

CPPSRC += 	gtest-all.cpp \
          	gmock-all.cpp \


INCDIR += 	$(UNIT_TESTS_DIR)/googletest/googlemock/include \
          	$(UNIT_TESTS_DIR)/googletest/googletest \
          	$(UNIT_TESTS_DIR)/googletest/googletest/include \

PCH_DIR = ../firmware/pch
PCHSRC = $(PCH_DIR)/pch.h
PCHSUB = unit_tests

include $(PROJECT_DIR)/rusefi_rules.mk

# Compiler options here.
ifeq ($(USE_OPT),)
# -O2 is needed for mingw, without it there is a linking issue to isnanf?!?!
  #USE_OPT = $(RFLAGS) -O2 -fgnu89-inline -ggdb -fomit-frame-pointer -falign-functions=16 -std=gnu99 -Werror-implicit-function-declaration -Werror -Wno-error=pointer-sign -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=sign-compare -Wno-error=unused-parameter -Wno-error=missing-field-initializers
  USE_OPT = -c -Wall -O0 -ggdb -g
  USE_OPT += -Werror=missing-field-initializers
endif

ifeq ($(COVERAGE),yes)
	USE_OPT += -fprofile-arcs -ftest-coverage
endif


#TODO! this is a nice goal
#USE_OPT += $(RUSEFI_OPT)
#USE_OPT += -Wno-error=format= -Wno-error=register -Wno-error=write-strings

# See explanation in main firmware Makefile for these three defines
USE_OPT += -DEFI_UNIT_TEST=1 -DEFI_PROD_CODE=0 -DEFI_SIMULATOR=0

# Enable address sanitizer, but not on Windows since x86_64-w64-mingw32-g++ doesn't support it.
ifneq ($(OS),Windows_NT)
	USE_OPT += -fsanitize=address
endif

# Pretend we are all different hardware so that all canned engine configs are included
USE_OPT += -DHW_MICRO_RUSEFI=1 -DHW_PROTEUS=1 -DHW_FRANKENSO=1 -DHW_HELLEN=1

ifeq ($(CCACHE_DIR),)
 $(info No CCACHE_DIR)
else
 $(info CCACHE_DIR is ${CCACHE_DIR})
 CCPREFIX=ccache
endif

# C specific options here (added to USE_OPT).
ifeq ($(USE_COPT),)
  USE_COPT = -std=gnu99 -fgnu89-inline
endif

# C++ specific options here (added to USE_OPT).
ifeq ($(USE_CPPOPT),)
  USE_CPPOPT = -std=gnu++17 -fno-rtti -fpermissive -fno-use-cxa-atexit
endif

# Enable this if you want the linker to remove unused code and data
ifeq ($(USE_LINK_GC),)
  USE_LINK_GC = yes
endif

# If enabled, this option allows to compile the application in THUMB mode.
ifeq ($(USE_THUMB),)
  USE_THUMB = no
endif

# Enable this if you want to see the full log while compiling.
ifeq ($(USE_VERBOSE_COMPILE),)
  USE_VERBOSE_COMPILE = no
endif

# C sources to be compiled in ARM mode regardless of the global setting.
ACSRC =

# C++ sources to be compiled in ARM mode regardless of the global setting.
ACPPSRC =

# C sources to be compiled in THUMB mode regardless of the global setting.
TCSRC =

# C sources to be compiled in THUMB mode regardless of the global setting.
TCPPSRC =

# List ASM source files here
ASMSRC =

##############################################################################
# Compiler settings
#

# It looks like cygwin build of mingwg-w64 has issues with gcov runtime :(
# mingw-w64 is a project which forked from mingw in 2007 - be careful not to confuse these two.
# In order to have coverage generated please download from https://mingw-w64.org/doku.php/download/mingw-builds
# Install using mingw-w64-install.exe instead of similar thing packaged with cygwin
# Both 32 bit and 64 bit versions of mingw-w64 are generating coverage data.

ifeq ($(OS),Windows_NT)
ifeq ($(USE_MINGW32_I686),)
#this one is 64 bit
  TRGT = x86_64-w64-mingw32-
else
#this one was 32 bit
  TRGT = i686-w64-mingw32-
endif
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
OD   = $(TRGT)objdump
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary

# ARM-specific options here
AOPT =

# THUMB-specific options here
TOPT = -mthumb -DTHUMB

# Define C warning options here
CWARN = -Wall -Wextra -Wstrict-prototypes -pedantic -Wmissing-prototypes -Wold-style-definition

# Define C++ warning options here
CPPWARN = -Wall -Wextra -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -Wno-format -Wno-unused-parameter -Wno-unused-private-field

#
# Compiler settings
##############################################################################

##############################################################################
# Start of default section
#

# List all default ASM defines here, like -D_DEBUG=1
DADEFS =

# List all default directories to look for include files here
DINCDIR =

# List the default directory to look for the libraries here
DLIBDIR =

# List all default libraries here
ifeq ($(OS),Windows_NT)
  # Windows
  DLIBS = -static-libgcc -static -static-libstdc++
else
  # Linux
  DLIBS = -pthread
endif

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

ifeq ($(COVERAGE),yes)
	ULIBS += --coverage
endif

ifneq ($(OS),Windows_NT)
	ULIBS += -fsanitize=address
endif

#
# End of user defines
##############################################################################

# Define project name here
PROJECT = rusefi_test

ifeq ("$(wildcard $(UNIT_TESTS_DIR)/googletest/LICENSE)","")
$(info Invoking "git submodule update --init")
$(shell git submodule update --init)
$(info Invoked "git submodule update --init")
# make is not happy about newly checked out module for some reason but next invocation would work
$(error Please run 'make' again. Please make sure you have 'git' command in PATH)
endif

include $(UNIT_TESTS_DIR)/rules.mk
include $(PROJECT_DIR)/rusefi_pch.mk
