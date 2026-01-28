##############################################################################
# see readme.md
# see setup_linux_environment.sh but Windows is also totally fine
#
##############################################################################
# The most advanced open source ECU has the most advanced Makefile!
# Here we do a bit of code generation, compile custom bootloader and embedded firmware and even build user artifacts!
#
# In order for Make to know which board you are trying to build for, it needs some parameters.
# BOARD_DIR - relative to the firmware dir, will be set to config/boards/f407-discovery if left empty
# SHORT_BOARD_NAME - will be set to f407-discovery if left empty
# PROJECT_BOARD - will be set to SHORT_BOARD_NAME if left empty
#
# You can set these as environment variables like this:
# source config/boards/common_script_read_meta_env.inc config/boards/f407-discovery/meta-info.env
# source config/boards/common_script_read_meta_env.inc config/boards/subaru_eg33/meta-info-subaru_eg33_f7.env
# source config/boards/common_script_read_meta_env.inc firmware/config/boards/hellen/hellen154hyundai_f7/meta-info-hellen154hyundai_f7.env
#
# There is also a script to compile the firmware to avoid messing with your environment variables.
# ./bin/compile.sh config/boards/f407-discovery/meta-info.env
# Or to package the bundles:
# ./bin/compile.sh -b config/boards/f407-discovery/meta-info.env
# If not provided with a path to a meta-info file, compile.sh will let you select a target.
#
# If you wish to call make directly, here are the available commands:
#
# make docs-enums	Generate docs and enums
# make config			Generate docs, enums, and configs
# make						Generate docs, enums, configs, and build firmware
# make bundle			Generate docs, enums, configs, build bootloader, build firmware, and package full bundle
# make autoupdate	Generate docs, enums, configs, build bootloader, build firmware, and package autoupdate bundle
# make bundles		Generate docs, enums, configs, build bootloader, build firmware, and package both full and autoupdate bundles
# make bootloader Generate docs, enums, configs, and build bootloader
# make ../simulator/build/rusefi_simulator.exe build Windows simulator (even if invoked on Linux)
# bin/compile.sh config/boards/f407-discovery/meta-info.env ../simulator/build/rusefi_simulator.linux build Linux simulator
# make ../simulator/build/rusefi_simulator.both build _both_ Windows and Linux simulator (do we need that? shall we drop that?)
#

CHIBIOS = ChibiOS
RULESPATH = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk
RULESFILE = $(RULESPATH)/rules.mk

ifneq ("$(wildcard $(../.git))","")
$(error "../.git not found. We expect source code to be cloned not downloaded as zip file.")
endif

# Define project name here
PROJECT = rusefi
PROJECT_DIR = .

# Configure precompiled header
PCH_DIR = $(PROJECT_DIR)/pch
PCHSRC = $(PCH_DIR)/pch.h
PCHSUB = firmware

# Imported source files and paths
CHIBIOS_CONTRIB = ChibiOS-Contrib

SUBMODULES = $(CHIBIOS) $(CHIBIOS_CONTRIB) libfirmware

ifneq ($(filter 1,$(foreach d,$(SUBMODULES),$(if $(wildcard $(d)/*),,1))),)
$(info Submodule not found. Chibios: Invoking "git submodule update --init")
DISCARD := $(shell git submodule update --init)
$(info Invoked "git submodule update --init")
endif

include $(PROJECT_DIR)/common_libfirmware.mk

include rusefi.mk

ifeq ($(DEBUG_LEVEL_OPT),)
  # Default to a release build - but leave in debug info.  It doesn't change the hex/bin output.
  DEBUG_LEVEL_OPT = -Os -ggdb -g
  # Comment the above and uncomment the line below if you want to debug the firmware (it's difficult-to-impossible
  # to debug firmware with optimization enabled).
  # DEBUG_LEVEL_OPT = -O0 -ggdb -g
  # without optimization many configurations do not fit into flash. Compiling without Lua is one of the way to get a debug image
  # DEBUG_LEVEL_OPT += -DEFI_LUA=FALSE
endif

# Compiler options here.
# yes we have two kinds of EXTRA_*PARAMS so that we can define those in two different places independently
#
ifeq ($(USE_OPT),)
  USE_OPT = $(EXTRA_PARAMS) $(EXTRA_2_PARAMS) $(EXTRA_3_PARAMS) $(DEBUG_LEVEL_OPT) $(RFLAGS) -fomit-frame-pointer -fsingle-precision-constant  -fno-inline-functions
endif

# EFI_UNIT_TEST determines if we are running in a unit test (hide things from hw/sim)
# EFI_PROD_CODE determines if we are running on real hardware (hide things from tests/sim)
# EFI_SIMULATOR determines if we are running in the simulator (hide things from hw/tests)
USE_OPT += $(RUSEFI_OPT) -DEFI_UNIT_TEST=0 -DEFI_PROD_CODE=1 -DEFI_SIMULATOR=0

# signalling to libfirmware that we have criticalError() method
USE_OPT += -DWE_HAVE_CRITICAL_ERROR_METHOD

# C specific options here (added to USE_OPT).
ifeq ($(USE_COPT),)
  USE_COPT = -fgnu89-inline -std=gnu99 -Wno-error=implicit-fallthrough
endif

# C++ specific options here (added to USE_OPT).
ifeq ($(USE_CPPOPT),)
  USE_CPPOPT  = -std=c++20 -Wno-register -fno-rtti -fno-threadsafe-statics -fno-exceptions -fno-use-cxa-atexit
  # gcc-10 c++ 20 depricated uses of volatile errors
  USE_CPPOPT += -Wno-deprecated
endif

# Hellen is one of the boards which cares
USE_CPPOPT += -Wno-error=tautological-compare

USE_CPPOPT += $(RUSEFI_CPPOPT)

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
  # ChibiOS has not yet migrated to gcc12 syntax of '-flto' thus some weird code here
  #
  # drama: in case of 'syntax error near unexpected token' on Windows either comment out the two lines below
  # or just more your GCC into a folder without spaces in the path like 'C:\stuff\arm-gcc-12.2'
  #
  USE_LTO = no
  USE_OPT += -flto=auto
endif

ifeq ($(OS),Windows_NT)
   USE_OPT += -DIS_WINDOWS_COMPILER=1
else
   USE_OPT += -DIS_WINDOWS_COMPILER=0
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
# without USE_SMART_BUILD all ChibiOS (including all drivers) are built. And all drivers includes get included.
ifeq ($(USE_SMART_BUILD),)
  USE_SMART_BUILD = no
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

BOARD_DIR := ./$(BOARD_DIR)
BOARDINC = $(BOARD_DIR)
include $(BOARD_DIR)/board.mk
BOARDCPPSRC += $(BOARDS_DIR)/board_id.cpp
ifeq (,$(filter clean,$(MAKECMDGOALS)))
ifeq ($(SHORT_BOARD_NAME),)
$(error SHORT_BOARD_NAME not set, something wrong with your meta-info.env file)
endif
endif
DDEFS += -DSHORT_BOARD_NAME=$(SHORT_BOARD_NAME)

# Include various ChibiOS mk files
# Licensing files.
include $(CHIBIOS)/os/license/license.mk

# should be after 'board.mk'
# includes ChibiOS's platform.mk for specified CPU
include rusefi_rules.mk

# Startup files.
include $(CPU_STARTUP_DIR)
# HAL-OSAL files (optional).
include $(CHIBIOS_CONTRIB)/os/hal/hal.mk
include $(CPU_PLATFORM)
include $(CHIBIOS)/os/hal/osal/rt-nil/osal.mk
# RTOS files (optional).
include $(CHIBIOS)/os/rt/rt.mk
include $(CHIBIOS)/os/common/ports/ARMv7-M/compilers/GCC/mk/port.mk
# EX files (optional).
ifeq ($(EFI_ONBOARD_MEMS_LIS2DW12),yes)
  DDEFS += -DEFI_ONBOARD_MEMS_LIS2DW12=TRUE
  include $(CHIBIOS)/os/ex/devices/ST/lis2dw12.mk
endif
ifeq ($(EFI_ONBOARD_MEMS_LIS2DH12),yes)
  DDEFS += -DEFI_ONBOARD_MEMS_LIS2DH12=TRUE
  include $(CHIBIOS)/os/ex/devices/ST/lsm303agr.mk
endif
ifeq ($(EFI_ONBOARD_MEMS_LIS302DL),yes)
  DDEFS += -DEFI_ONBOARD_MEMS_LIS302DL=TRUE
  include $(CHIBIOS)/os/ex/devices/ST/lis302dl.mk
endif
ifeq ($(EFI_ONBOARD_MEMS_LIS3DSH),yes)
  DDEFS += -DEFI_ONBOARD_MEMS_LIS3DSH=TRUE
  include $(CHIBIOS)/os/ex/devices/ST/lis3dsh.mk
endif
include $(CHIBIOS)/os/hal/lib/streams/streams.mk
include $(CHIBIOS)/os/various/cpp_wrappers/chcpp.mk

ifeq ($(LWIP),yes)
  include $(CHIBIOS)/os/various/lwip_bindings/lwip.mk
endif

include $(CHIBIOS)/os/hal/lib/complex/mfs/hal_mfs.mk

ifeq ($(USE_FATFS),yes)
  DDEFS += -DEFI_SUPPORT_FATFS=TRUE
  include $(CHIBIOS)/os/various/fatfs_bindings/fatfs.mk
endif

#Now include RusEFI mk files
include console/binary/tunerstudio.mk
ifeq ($(USE_FATFS),yes)
  include $(PROJECT_DIR)/ext/FatFS/fatfs.mk
endif

ifeq ($(USE_WIFI),yes)
  include $(PROJECT_DIR)/ext/atwinc1500/atwinc1500.mk
  DDEFS += -DEFI_WIFI=1
endif

include $(PROJECT_DIR)/hw_layer/mass_storage/mass_storage.mk
include $(PROJECT_DIR)/common.mk
include $(PROJECT_DIR)/controllers/modules/modules.mk

ifeq ($(USE_OPENBLT),yes)
  # Reserve start of flash for OpenBLT
  USE_OPT += -Wl,--defsym=HAS_BOOTLOADER=1
  DDEFS += -DEFI_USE_OPENBLT=TRUE
endif

$(info PROJECT_BOARD: $(PROJECT_BOARD))
$(info BOARD_DIR:     $(BOARD_DIR))
$(info PROJECT_CPU:   $(PROJECT_CPU))
$(info SHORT_BOARD_NAME: $(SHORT_BOARD_NAME))
$(info CPU_HWLAYER:   $(CPU_HWLAYER))
$(info CONFDIR:       $(CONFDIR))
$(info LDSCRIPT:      $(LDSCRIPT))

# C sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
# gcc_version_check.c goes first to throw error as soon as possible
CSRC =  gcc_version_check.c \
  $(ALLCSRC) \
  $(BOARDSRC) \
  $(CHIBIOS_CONTRIB)/os/various/ramdisk.c \
  $(CHIBIOS)/os/various/evtimer.c \
  $(DEV_SRC) \
  $(BOOTLOADERSRC) \
  ${HW_MASS_STORAGE_SRC_C} \
  $(HW_LAYER) \
  $(HW_LAYER_PORT) \
  $(HW_LAYER_DRIVERS_CORE) \
  $(HW_LAYER_DRIVERS) \
  $(RUSEFI_LIB_C) \
  $(PROJECT_DIR)/hw_layer/main_hardfault.c

# C++ sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CPPSRC = \
  configuration_check.cpp \
  $(ALLCPPSRC) \
  $(BOARDCPPSRC) \
  $(DEV_SRC_CPP) \
  $(HW_LAYER_DRIVERS_CORE_CPP) \
  $(HW_LAYER_DRIVERS_CPP) \
  $(CONSOLE_SRC_CPP) \
  $(RUSEFI_LIB_CPP) \
  $(MODULES_CPPSRC) \
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
  $(RUSEFIASM) \
  $(PROJECT_DIR)/hw_layer/main_hardfault_asm.S

#
# WARNING! order of variables is important here - for instance cypress own folders should go before default folders
#
INCDIR += \
  $(PCH_DIR) \
  $(BOARDINC) \
  $(ALLINC) \
  $(MODULES_INC) \
  $(TESTINC) \
  $(CHIBIOS)/os/various \
  $(RUSEFI_LIB_INC) \
  $(CONFDIR) \
  ext/FatFS \
  $(PROJECT_DIR)/hw_layer/mass_storage \
  $(HW_INC) \
  $(PROJECT_DIR)/development/hw_layer \
  development/test \
  $(CHIBIOS_CONTRIB)/os/various
# contrib/os/various must go at the end because it also contains pid.h which conflicts with our pid.h

#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#

ifeq ($(CROSS_COMPILE),)
  TRGT = arm-none-eabi-
else
  TRGT = $(CROSS_COMPILE)
endif
CC   = $(TRGT)gcc
CPPC = $(TRGT)g++
# Enable loading with g++ only if you need C++ runtime support.
# NOTE: You can use C++ even without C++ support if you are careful. C++
#       runtime support makes code size explode.
LD   = $(CC)
#LD   = $(CPPC)
CP   = $(TRGT)objcopy
AS   = $(CC) -x assembler-with-cpp
AR   = $(TRGT)ar
OD   = $(TRGT)objdump
SZ   = $(TRGT)size
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary
# for srec files see bundle.mk

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
ULIBS = -lm --specs=nano.specs -Wl,--sort-section=alignment

#
# End of user defines
##############################################################################

include $(RULESFILE)

include target_sentinel.mk

# this one would include ../java_tools/java_tools.mk for gradlew for config and console and plugin jars
include rusefi_config.mk

include docs_enums.mk

# Enable precompiled header
include rusefi_pch.mk

include bundle.mk

.PHONY: CLEAN_RULE_HOOK CLEAN_PCH_HOOK CLEAN_BUNDLE_HOOK

CLEAN_RULE_HOOK: CLEAN_PCH_HOOK CLEAN_BUNDLE_HOOK

CLEAN_PCH_HOOK:

CLEAN_BUNDLE_HOOK:

POST_MAKE_ALL_RULE_HOOK: $(BUILDDIR)/$(PROJECT).elf
	@java -jar ../java_tools/gcc_map_reader.jar $(BUILDDIR)/$(PROJECT).map | grep Total || echo Unable to run gcc_map_reader
	@$(TRGT)objdump -h $(BUILDDIR)/$(PROJECT).elf | grep -w ram4

VPATH     = $(patsubst ./%,%,$(SRCPATHS))
