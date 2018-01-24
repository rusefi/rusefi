##############################################################################
# Build global options
# NOTE: Can be overridden externally.
#

PROJECT_DIR = ../firmware
#CHIBIOS = $(PROJECT_DIR)/chibios

# Compiler options here.
ifeq ($(USE_OPT),)
# -O2 is needed for mingw, without it there is a linking issue to isnanf?!?!
  #USE_OPT = $(RFLAGS) -O2 -fgnu89-inline -ggdb -fomit-frame-pointer -falign-functions=16 -std=gnu99 -Werror-implicit-function-declaration -Werror -Wno-error=pointer-sign -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=sign-compare -Wno-error=unused-parameter -Wno-error=missing-field-initializers
  USE_OPT = -c -Wall -O0 -ggdb -g3
endif

# C specific options here (added to USE_OPT).
ifeq ($(USE_COPT),)
  USE_COPT = -std=gnu99 -fgnu89-inline
endif

# C++ specific options here (added to USE_OPT).
ifeq ($(USE_CPPOPT),)
  USE_CPPOPT = -fno-rtti -fpermissive -fno-exceptions -fno-use-cxa-atexit
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
include $(PROJECT_DIR)/controllers/algo/algo.mk
include $(PROJECT_DIR)/controllers/core/core.mk
include $(PROJECT_DIR)/controllers/math/math.mk
include $(PROJECT_DIR)/controllers/system/system.mk
include $(PROJECT_DIR)/controllers/sensors/sensors.mk
include $(PROJECT_DIR)/controllers/trigger/trigger.mk
include $(PROJECT_DIR)/hw_layer/hw_layer.mk
include test.mk

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
	$(TEST_SRC_C)

# C++ sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CPPSRC = $(UTILSRC_CPP) \
	$(CONTROLLERS_ALGO_SRC_CPP) \
	$(TRIGGER_DECODERS_SRC_CPP) \
	$(ENGINES_SRC_CPP) \
	$(CONTROLLERS_CORE_SRC_CPP) \
	$(CONTROLLERS_MATH_SRC_CPP) \
	$(CONTROLLERS_SENSORS_SRC_CPP) \
	$(TEST_SRC_CPP) \
	$(SYSTEMSRC_CPP) \
	$(PROJECT_DIR)/controllers/error_handling.cpp \
	$(TRIGGER_SRC_CPP) \
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

CHIBIOS=$(PROJECT_DIR)/ChibiOS4

INCDIR = . \
	$(PROJECT_DIR)/util \
	$(PROJECT_DIR)/config/engines \
	$(PROJECT_DIR)/controllers \
	$(PROJECT_DIR)/controllers/sensors \
	$(PROJECT_DIR)/controllers/algo \
	$(PROJECT_DIR)/controllers/core \
	$(PROJECT_DIR)/controllers/math \
	$(PROJECT_DIR)/controllers/system \
	$(PROJECT_DIR)/controllers/trigger \
	$(PROJECT_DIR)/controllers/trigger/decoders \
	$(PROJECT_DIR)/ext_algo \
	$(CHIBIOS)/os/rt/include \
	$(CHIBIOS)/os/rt/various \
	$(CHIBIOS)/os/hal/lib/streams \
	$(CHIBIOS)/os/hal/include \
	$(PROJECT_DIR)/hw_layer \
	test_data_structures \
	test_basic_math
	

#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#

#MCU  = cortex-m4

ifeq ($(OS),Windows_NT)
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

# List all default ASM defines here, like -D_DEBUG=1
DADEFS =

# List all default directories to look for include files here
DINCDIR =

# List the default directory to look for the libraries here
DLIBDIR =

# List all default libraries here
DLIBS = -static-libgcc -static-libstdc++

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

include rules.mk
