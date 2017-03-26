##############################################################################
# Build global options
# NOTE: Can be overridden externally.
#

# Compiler options here.
ifeq ($(USE_OPT),)
# this config if debugging is needed, but the binary is about 50M
# USE_OPT = -c -Wall -O0 -ggdb -g3 -Werror-implicit-function-declaration -Werror -Wno-error=pointer-sign -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=sign-compare -Wno-error=unused-parameter -Wno-error=missing-field-initializers -Wno-error=write-strings
# this config producec a smaller binary file
USE_OPT = -Os 
endif

ifeq ($(OS),Windows_NT)
else
USE_OPT +=  -m32
endif

# C specific options here (added to USE_OPT).
ifeq ($(USE_COPT),)
USE_COPT = -std=gnu99 -fgnu89-inline
endif

ifeq ($(OS),Windows_NT)
else
USE_COPT += -Wno-error=attributes -Wno-error=implicit-function-declaration -include stdio.h
endif

# C++ specific options here (added to USE_OPT).
ifeq ($(USE_CPPOPT),)
USE_CPPOPT = -std=c++11 -fno-rtti -fpermissive -fno-exceptions -fno-use-cxa-atexit
endif

ifeq ($(OS),Windows_NT)
else
USE_CPPOPT += -include cstring -include cstdio -include cstdlib
endif

# Enable this if you want the linker to remove unused code and data
ifeq ($(USE_LINK_GC),)
  USE_LINK_GC = yes
endif

# Enable this if you want to see the full log while compiling.
ifeq ($(USE_VERBOSE_COMPILE),)
  USE_VERBOSE_COMPILE = yes
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
PROJECT = rusefi_simulator
PROJECT_DIR = ../firmware
CHIBIOS = ../firmware/ChibiOS

# Imported source files and paths
include $(CHIBIOS)/os/hal/boards/simulator/board.mk
include $(CHIBIOS)/os/hal/hal.mk
include $(CHIBIOS)/os/hal/osal/rt/osal.mk
include $(CHIBIOS)/os/rt/ports/SIMIA32/compilers/GCC/port.mk
include $(CHIBIOS)/os/rt/rt.mk

ifeq ($(OS),Windows_NT)
  include ${CHIBIOS}/os/hal/ports/simulator/win32/platform.mk
else
  include ${CHIBIOS}/os/hal/ports/simulator/Posix/platform.mk
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

# Define linker script file here
#LDSCRIPT= config/system/STM32F407xG.ld
#LDSCRIPT= $(PORTLD)/STM32F407xG_CCM.ld

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
        $(CHIBIOS)/os/various/shell.c \
        $(UTILSRC) \
        main.c

# C++ sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CPPSRC = $(UTILSRC_CPP) \
	$(CONTROLLERS_ALGO_SRC_CPP) \
	$(PROJECT_DIR)/controllers/settings.cpp \
	$(PROJECT_DIR)/controllers/engine_controller.cpp \
	$(PROJECT_DIR)/controllers/error_handling.cpp \
	$(PROJECT_DIR)/development/sensor_chart.cpp \
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
	$(PROJECT_DIR)/controllers/map_averaging.cpp \
	$(PROJECT_DIR)/development/trigger_emulator.cpp \
	$(TEST_SRC_CPP)


# List ASM source files here
ASMSRC = $(PORTASM)

INCDIR = . \
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

#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#

#MCU  = cortex-m4


ifeq ($(OS),Windows_NT)
  #Cygwin64 is used with mingw64 32-bit version

  #TRGT = i686-w64-mingw32-
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
OD   = $(TRGT)objdump
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary

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

# List all default ASM defines here, like -D_DEBUG=1
DADEFS =

# List all default directories to look for include files here
DINCDIR =

# List the default directory to look for the libraries here
DLIBDIR =

# List all default libraries here
ifeq ($(OS),Windows_NT)
  DLIBS = -static-libgcc -static-libstdc++ -lws2_32
else
  DLIBS = -static-libgcc -lgcc -static-libstdc++ -m32
endif


#
# End of default section
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
ULIBS = -lm

#
# End of user defines
##############################################################################

include rules.mk
