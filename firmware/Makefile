##############################################################################
# Build global options
# NOTE: Can be overridden externally.
#

PROJECT_DIR = .
CHIBIOS = chibios

# Compiler options here.
ifeq ($(USE_OPT),)
  USE_OPT = $(RFLAGS) -O1 -fgnu89-inline -ggdb -fomit-frame-pointer -falign-functions=16 -std=gnu99 -Werror-implicit-function-declaration -Werror -Wno-error=pointer-sign -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=sign-compare -Wno-error=unused-parameter -Wno-error=missing-field-initializers
endif

# C specific options here (added to USE_OPT).
ifeq ($(USE_COPT),)
  USE_COPT = 
endif

# C++ specific options here (added to USE_OPT).
ifeq ($(USE_CPPOPT),)
  USE_CPPOPT = -std=c++11 -fno-rtti -fno-exceptions -fno-use-cxa-atexit
endif

# Enable this if you want the linker to remove unused code and data
ifeq ($(USE_LINK_GC),)
  USE_LINK_GC = yes
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

USE_FPU = yes

# Enables the use of FPU on Cortex-M4.
# Enable this if you really want to use the STM FWLib.
ifeq ($(USE_FPU),)
  USE_FPU = no
endif

# Enable this if you really want to use the STM FWLib.
ifeq ($(USE_FWLIB),)
  USE_FWLIB = no
endif

# List all default C defines here, like -D_DEBUG=1
DDEFS =

#
# Architecture or project specific options
##############################################################################

##############################################################################
# Project, sources and paths
#

# Define project name here
PROJECT = rusefi

#PROJECT_BOARD = OLIMEX_STM32_E407
ifneq ($(PROJECT_BOARD),OLIMEX_STM32_E407)
  PROJECT_BOARD = ST_STM32F4_DISCOVERY
endif
DDEFS += -D$(PROJECT_BOARD)

# Imported source files and paths
include $(CHIBIOS)/boards/$(PROJECT_BOARD)/board.mk
include $(CHIBIOS)/os/hal/platforms/STM32F4xx/platform.mk
include $(CHIBIOS)/os/hal/hal.mk
include $(CHIBIOS)/os/ports/GCC/ARMCMx/STM32F4xx/port.mk
include $(CHIBIOS)/os/kernel/kernel.mk
include $(CHIBIOS)/os/various/cpp_wrappers/kernel.mk

include $(CHIBIOS)/os/various/fatfs_bindings/fatfs.mk
include console/tunerstudio/tunerstudio.mk
include ext/ext.mk
include hw_layer/hw_layer.mk
include emulation/emulation.mk
include controllers/controllers.mk

include $(PROJECT_DIR)/util/util.mk
include $(PROJECT_DIR)/config/engines/engines.mk
include $(PROJECT_DIR)/controllers/algo/algo.mk
include $(PROJECT_DIR)/controllers/core/core.mk
include $(PROJECT_DIR)/controllers/math/math.mk
include $(PROJECT_DIR)/controllers/sensors/sensors.mk
include $(PROJECT_DIR)/controllers/system/system.mk
include $(PROJECT_DIR)/controllers/trigger/trigger.mk
include $(PROJECT_DIR)/console/console.mk
include $(PROJECT_DIR)/console_util/console_util.mk


# Define linker script file here
LDSCRIPT= config/system/STM32F407xG_CCM.ld
#LDSCRIPT= $(PORTLD)/STM32F407xG_CCM.ld

# C sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CSRC = $(PORTSRC) \
	$(KERNSRC) \
	$(VARIOUSSRC) \
	chibios/os/various/chprintf.c \
	chibios/os/various/memstreams.c \
	chibios/os/various/chrtclib.c  \
	$(UTILSRC) \
	$(ENGINES_SRC) \
	$(CONSOLESRC) \
	$(TUNERSTUDIOSRC) \
	$(CONSOLEUTILSRC) \
	$(HALSRC) \
	$(EMULATIONSRC) \
	$(HW_LAYERSRC) \
	$(CONTROLLERSSRC) \
	$(CONTROLLERS_ALGO_SRC) \
	$(CONTROLLERS_CORE_SRC) \
	$(CONTROLLERS_MATH_SRC) \
	$(CONTROLLERS_SENSORS_SRC) \
	$(PLATFORMSRC) \
	$(BOARDSRC) \
	$(FATFSSRC) \
	$(TRIGGER_SRC) \
	$(SYSTEMSRC)

# C++ sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CPPSRC = $(CHCPPSRC) \
	$(TRIGGER_SRC_CPP) \
	$(TRIGGER_DECODERS_SRC_CPP) \
	$(EMULATIONSRC_CPP) \
	$(CONTROLLERS_ALGO_SRC_CPP) \
	$(SYSTEMSRC_CPP) \
	$(ENGINES_SRC_CPP) \
	$(HW_LAYER_SRC_CPP) \
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
	config/system \
	config/engines \
	config/boards \
	config \
	chibios/os/various \
	ext \
	ext_algo \
	util \
	console_util \
	console \
	console/tunerstudio \
	hw_layer \
	hw_layer/serial_over_usb \
	hw_layer/algo \
	hw_layer/lcd \
	emulation/hw_layer \
	emulation \
	controllers \
	controllers/sensors \
	controllers/system \
	controllers/algo \
	controllers/core \
	controllers/math \
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

ifeq ($(USE_FPU),yes)
  USE_OPT += -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -fsingle-precision-constant
  DDEFS += -DCORTEX_USE_FPU=TRUE
else
  DDEFS += -DCORTEX_USE_FPU=FALSE
endif

ifeq ($(USE_FWLIB),yes)
  include $(CHIBIOS)/ext/stm32lib/stm32lib.mk
  CSRC += $(STM32SRC)
  INCDIR += $(STM32INC)
  USE_OPT += -DUSE_STDPERIPH_DRIVER
endif

include $(CHIBIOS)/os/ports/GCC/ARMCMx/rules.mk
