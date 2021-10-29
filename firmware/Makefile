##############################################################################
# see readme.md
# see setup_linux_environment.sh but Windows is also totally fine
#

CHIBIOS = ChibiOS
RULESPATH = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk
RULESFILE = $(RULESPATH)/rules.mk

include rusefi_rules.mk

# Define project name here
PROJECT = rusefi
PROJECT_DIR = .

# Configure precompiled header
PCH_DIR = $(PROJECT_DIR)/pch
PCHSRC = $(PCH_DIR)/pch.h

# Imported source files and paths
CHIBIOS_CONTRIB = ChibiOS-Contrib

include rusefi.mk

# by default EXTRA_PARAMS is empty and we create 'debug' version of the firmware with additional assertions and statistics
# for 'release' options see 'clean_compile_two_versions.bat' file

ifeq ($(DEBUG_LEVEL_OPT),)
  # Default to a release build - but leave in debug info.  It doesn't change the hex/bin output.
  DEBUG_LEVEL_OPT = -O2 -ggdb -g
  # Comment the above and uncomment the line below if you want to debug the firmware (it's difficult-to-impossible
  # to debug firmware with optimization enabled).
  # DEBUG_LEVEL_OPT = -O0 -ggdb -g
endif


ifeq ($(CCACHE_DIR),)
 $(info No CCACHE_DIR)
else
 $(info CCACHE_DIR is ${CCACHE_DIR})
 CCPREFIX=ccache
endif


# Compiler options here.
ifeq ($(USE_OPT),)
  USE_OPT = $(EXTRA_PARAMS) $(DEBUG_LEVEL_OPT) $(RFLAGS) -fomit-frame-pointer -falign-functions=16 -fsingle-precision-constant
endif

# EFI_UNIT_TEST determines if we are running in a unit test (hide things from hw/sim)
# EFI_PROD_CODE determines if we are running on real hardware (hide things from tests/sim)
# EFI_SIMULATOR determines if we are running in the simulator (hide things from hw/tests)
USE_OPT += $(RUSEFI_OPT) -DEFI_UNIT_TEST=0 -DEFI_PROD_CODE=1 -DEFI_SIMULATOR=0

# C specific options here (added to USE_OPT).
ifeq ($(USE_COPT),)
  USE_COPT = -fgnu89-inline -std=gnu99 -Wno-error=implicit-fallthrough
endif

# C++ specific options here (added to USE_OPT).
ifeq ($(USE_CPPOPT),)
  USE_CPPOPT = -std=c++17 -Wno-register -fno-rtti -fno-threadsafe-statics -fno-exceptions -fno-use-cxa-atexit -Winvalid-pch
endif

# Hellen is one of the boards which cares
USE_CPPOPT += -Wno-error=tautological-compare

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
# without USE_SMART_BUILD all ChibiOS (including all drivers) are builded. And all drivers includes get included.
ifeq ($(USE_SMART_BUILD),)
  USE_SMART_BUILD = no
endif

ifeq ($(USE_BOOTLOADER),)
  USE_BOOTLOADER = no
endif

ifeq ($(USE_OPENBLT),)
  USE_OPENBLT = no
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

BOARDS_DIR = $(PROJECT_DIR)/config/boards

include $(PROJECT_DIR)/config/boards/$(PROJECT_BOARD)/board.mk

# Include various ChibiOS mk files
# Licensing files.
include $(CHIBIOS)/os/license/license.mk
# Startup files.
include $(CPU_STARTUP_DIR)
# HAL-OSAL files (optional).
include $(CHIBIOS_CONTRIB)/os/hal/hal.mk
include $(CPU_PLATFORM)
include $(CHIBIOS)/os/hal/osal/rt-nil/osal.mk
# RTOS files (optional).
include $(CHIBIOS)/os/rt/rt.mk
include $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/mk/port_v7m.mk
# EX files (optional).
ifneq ($(USE_LIS302),no)
include $(CHIBIOS)/os/ex/devices/ST/lis302dl.mk
endif
include $(CHIBIOS)/os/hal/lib/streams/streams.mk
include $(CHIBIOS)/os/various/cpp_wrappers/chcpp.mk

ifeq ($(LWIP),yes)
	include $(CHIBIOS)/os/various/lwip_bindings/lwip.mk
endif

include $(CHIBIOS)/os/hal/lib/complex/mfs/hal_mfs.mk

ifeq ($(USE_FATFS),yes)
include $(CHIBIOS)/os/various/fatfs_bindings/fatfs.mk
endif

#Now include RusEFI mk files
include console/binary/tunerstudio.mk
ifeq ($(USE_FATFS),yes)
include $(PROJECT_DIR)/ext/fatfs.mk
endif

include $(PROJECT_DIR)/config/engines/engines.mk
include $(PROJECT_DIR)/console/console.mk
include $(PROJECT_DIR)/controllers/controllers.mk
include $(PROJECT_DIR)/controllers/lua/lua.mk
include $(PROJECT_DIR)/development/development.mk
include $(PROJECT_DIR)/hw_layer/hw_layer.mk
include $(PROJECT_DIR)/hw_layer/drivers/drivers.mk
include $(PROJECT_DIR)/hw_layer/sensors/sensors.mk
include $(PROJECT_DIR)/hw_layer/mass_storage/mass_storage.mk
include $(PROJECT_DIR)/init/init.mk
include $(PROJECT_DIR)/util/util.mk

ifeq ($(BOOTLOADERINC),)
  # include default bootloader code
  BOOTLOADERINC= $(PROJECT_DIR)/bootloader/
endif

ifeq ($(USE_BOOTLOADER),yes)
include $(PROJECT_DIR)/bootloader/bootloader.mk
endif

ifeq ($(USE_OPENBLT),yes)
  # Reserve start of flash for OpenBLT
  USE_OPT += -Wl,--defsym=BOOTLOADER=1
endif

$(info PROJECT_BOARD: $(PROJECT_BOARD))
$(info PROJECT_CPU:   $(PROJECT_CPU))
$(info CONFDIR:       $(CONFDIR))
$(info LDSCRIPT:      $(LDSCRIPT))

# C sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CSRC =  $(ALLCSRC) \
	$(CHIBIOS)/os/various/syscalls.c \
	$(CHIBIOS_CONTRIB)/os/various/ramdisk.c \
	$(CHIBIOS)/os/various/evtimer.c \
	$(TRIGGER_SRC) \
	$(TRIGGER_DECODERS_SRC) \
	$(DEV_SRC) \
	$(BOOTLOADERSRC) \
	${HW_MASS_STORAGE_SRC_C} \
	$(UTILSRC) \
	$(SYSTEMSRC) \
	$(ENGINES_SRC) \
	$(CONSOLESRC) \
	$(HW_LAYER_EMS) \
	$(HW_LAYER_DRIVERS_CORE) \
	$(HW_LAYER_DRIVERS) \
	$(CONTROLLERSSRC) \
	$(CONTROLLERS_ALGO_SRC) \
	$(CONTROLLERS_CORE_SRC) \
	$(CONTROLLERS_SENSORS_SRC) \
	main_hardfault.c

# C++ sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CPPSRC = $(ALLCPPSRC) \
	$(TRIGGER_SRC_CPP) \
	$(TRIGGER_DECODERS_SRC_CPP) \
	$(DEV_SRC_CPP) \
	$(CONTROLLERS_ALGO_SRC_CPP) \
	$(UTILSRC_CPP) \
	$(SYSTEMSRC_CPP) \
	$(ENGINES_SRC_CPP) \
	$(HW_LAYER_EMS_CPP) \
	$(HW_LAYER_DRIVERS_CORE_CPP) \
	$(HW_LAYER_DRIVERS_CPP) \
	$(HW_SENSORS_CPP) \
	$(TUNERSTUDIO_SRC_CPP) \
	$(CONSOLE_SRC_CPP) \
	$(CONTROLLERS_SENSORS_SRC_CPP) \
	$(CONTROLLERS_SRC_CPP) \
	$(CONTROLLERS_CORE_SRC_CPP) \
	$(CONTROLLERS_MATH_SRC_CPP) \
	$(INIT_SRC_CPP) \
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
ASMXSRC = $(ALLXASMSRC) \
	$(RUSEFIASM)

INCDIR = \
	$(PCH_DIR) \
	$(ALLINC) \
	$(TESTINC) \
	$(BOOTLOADERINC) \
	$(CHIBIOS)/os/various \
	$(CHIBIOS)/os/hal/lib/peripherals/sensors \
	$(CONFDIR) \
	config/engines \
	config/boards \
	config \
	ext \
	ext_algo \
	console_util \
	$(CONSOLE_INC) \
	$(HW_LAYER_INC) \
	$(PROJECT_DIR)/hw_layer/mass_storage \
	$(PROJECT_DIR)/hw_layer/serial_over_usb \
	$(PROJECT_DIR)/hw_layer/algo \
	$(PROJECT_DIR)/hw_layer/lcd \
	$(PROJECT_DIR)/hw_layer/mass_storage \
	$(HW_SENSORS_INC) \
	$(HW_INC) \
	$(HW_LAYER_DRIVERS_INC) \
	$(UTIL_INC) \
	init \
	$(DEVELOPMENT_DIR) \
	development/hw_layer \
	development/test \
	$(CONTROLLERS_INC) \
	$(CHIBIOS_CONTRIB)/os/various
	# contrib/os/various must go at the end because it also contains pid.h which conflicts with our pid.h

#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#

ifeq ($(MCU),)
	MCU = cortex-m4
endif

ifeq ($(CROSS_COMPILE),)
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
# OpenBLT flasher expects srec files
SREC = $(CP) -O srec

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

# Enable precompiled header
include rusefi_pch.mk
