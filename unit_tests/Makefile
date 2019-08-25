##############################################################################
# Build global options
# NOTE: Can be overridden externally.
#

PROJECT_DIR = ../firmware

# Compiler options here.
ifeq ($(USE_OPT),)
# As of Jan 2016 we are producing 64 bit unit tests on Windows which needs 64 bit GDB
# Previouslyt we had to download 32 bit version of GDB maybe from http://www.equation.com/servlet/equation.cmd?fa=gdb
#
# -O2 is needed for mingw, without it there is a linking issue to isnanf?!?!
  #USE_OPT = $(RFLAGS) -O2 -fgnu89-inline -ggdb -fomit-frame-pointer -falign-functions=16 -std=gnu99 -Werror-implicit-function-declaration -Werror -Wno-error=pointer-sign -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=sign-compare -Wno-error=unused-parameter -Wno-error=missing-field-initializers
  USE_OPT = -c -Wall -O0 -ggdb -g3
  USE_OPT += -fprofile-arcs -ftest-coverage
  USE_OPT += -Werror=missing-field-initializers
endif

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
  USE_CPPOPT = -std=gnu++17 -fno-rtti -fpermissive -fexceptions -fno-use-cxa-atexit
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

#
# Build global options
##############################################################################

##############################################################################
# Architecture or project specific options
#


# List all default C defines here, like -D_DEBUG=1
DDEFS =

#
# Architecture or project specific options
##############################################################################

##############################################################################
# Project, sources and paths
#

# Define project name here
PROJECT = rusefi_test

#PROJECT_BOARD = OLIMEX_STM32_E407
#ifneq ($(PROJECT_BOARD),OLIMEX_STM32_E407)
#  PROJECT_BOARD = ST_STM32F4_DISCOVERY
#endif
#DDEFS += -D$(PROJECT_BOARD)

# Imported source files and paths
include $(PROJECT_DIR)/util/util.mk
include $(PROJECT_DIR)/config/engines/engines.mk
include $(PROJECT_DIR)/controllers/controllers.mk
include $(PROJECT_DIR)/controllers/algo/algo.mk
include $(PROJECT_DIR)/controllers/core/core.mk
include $(PROJECT_DIR)/controllers/math/math.mk
include $(PROJECT_DIR)/controllers/system/system.mk
include $(PROJECT_DIR)/controllers/sensors/sensors.mk
include $(PROJECT_DIR)/controllers/trigger/trigger.mk
include $(PROJECT_DIR)/hw_layer/hw_layer.mk
include $(PROJECT_DIR)/hw_layer/drivers/drivers.mk
include $(PROJECT_DIR)/hw_layer/sensors/sensors.mk
include test.mk
include tests/tests.mk

# Define linker script file here
#LDSCRIPT= config/system/STM32F407xG.ld
#LDSCRIPT= $(PORTLD)/STM32F407xG_CCM.ld

# C sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CSRC =	$(UTILSRC) \
	$(CONTROLLERS_ALGO_SRC) \
	$(CONTROLLERS_CORE_SRC) \
	$(CONTROLLERS_MATH_SRC) \
	$(CONTROLLERS_SENSORS_SRC) \
	$(ENGINES_SRC) \
	$(HW_LAYER_DRIVERS_CORE) \
	$(TEST_SRC_C)

# C++ sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CPPSRC = $(UTILSRC_CPP) \
	gtest-all.cpp \
	gmock-all.cpp \
	$(CONTROLLERS_ALGO_SRC_CPP) \
	$(TRIGGER_DECODERS_SRC_CPP) \
	$(ENGINES_SRC_CPP) \
	$(CONTROLLERS_SRC_CPP) \
	$(CONTROLLERS_CORE_SRC_CPP) \
	$(CONTROLLERS_MATH_SRC_CPP) \
	$(CONTROLLERS_SENSORS_SRC_CPP) \
	$(FRAMEWORK_SRC_CPP) \
	$(TESTS_SRC_CPP) \
	$(SYSTEMSRC_CPP) \
	$(HW_LAYER_EMS_CPP) \
	$(HW_SENSORS_SRC) \
	$(TRIGGER_SRC_CPP) \
	$(PROJECT_DIR)/../unit_tests/main.cpp


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


INCDIR = . \
	$(UTIL_INC) \
	$(PROJECT_DIR)/config/engines \
	$(CONTROLLERS_INC) \
	$(PROJECT_DIR)/console \
	$(PROJECT_DIR)/controllers/sensors \
	$(PROJECT_DIR)/controllers/algo \
	$(PROJECT_DIR)/controllers/core \
	$(PROJECT_DIR)/controllers/math \
	$(PROJECT_DIR)/controllers/system \
	$(PROJECT_DIR)/controllers/trigger \
	$(PROJECT_DIR)/controllers/trigger/decoders \
	$(PROJECT_DIR)/ext_algo \
	$(PROJECT_DIR)/hw_layer \
	$(PROJECT_DIR)/hw_layer/algo \
	$(PROJECT_DIR)/hw_layer/sensors/ \
	$(HW_LAYER_DRIVERS_INC) \
	test_data_structures \
	googletest/googlemock/include \
	googletest/googletest \
	googletest/googletest/include \
	$(PROJECT_DIR)/../unit_tests/tests \
	$(PROJECT_DIR)/../unit_tests/test_basic_math
	

#
# Project, sources and paths
##############################################################################

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
CPPWARN = -Wall -Wextra

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
ULIBS += -lgcov --coverage

#
# End of user defines
##############################################################################

include rules.mk
