#
# this Makefile is intended to be invoked from one level above - see ..\compile_bootloader.sh
#

# by default EXTRA_PARAMS is empty and we create 'debug' version of the firmware with additional assertions and statistics
# for 'release' options see 'clean_compile_two_versions.bat' file

ifeq ($(DEBUG_LEVEL_OPT),)
  # this value would be used by default. For 'debug' configuration override with '-O0 -ggdb -g' or something along these lines
  DEBUG_LEVEL_OPT = -Os -ggdb -g
  DDEFS += -DEFI_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_STACK_CHECK=FALSE -DCH_DBG_FILL_THREADS=FALSE -DCH_DBG_THREADS_PROFILING=FALSE
endif

DDEFS += -DENABLE_AUTO_DETECT_HSE=TRUE -DSTM32_HSECLK=25000000

# disable some modules to shrink bootloader binary
DDEFS += -DEFI_BOOTLOADER
DDEFS += -DHAL_USE_EXT=FALSE -DHAL_USE_ICU=FALSE -DHAL_USE_PWM=FALSE -DHAL_USE_RTC=FALSE -DEF_LUA=FALSE
#disable ChibiOS flash driver and prevent header from include
DDEFS += -DHAL_USE_FLASH=FALSE

DDEFS += -DEFI_USE_UART_DMA=FALSE

DDEFS += -DEFI_USB_SERIAL=TRUE -DHAL_USE_USB_MSD=FALSE
# disable CAN
DDEFS += -DEFI_CAN_SUPPORT=FALSE

# Cache is disabled on F7, H7
DDEFS += -DSTM32_SRAM2_NOCACHE=FALSE -DSTM32_NOCACHE_SRAM1_SRAM2=FALSE -DSTM32_NOCACHE_SRAM3=FALSE

DDEFS += -DEFI_UNIT_TEST=0 -DEFI_PROD_CODE=1 -DEFI_SIMULATOR=0

# Compiler options here.
ifeq ($(USE_OPT),)
  USE_OPT = $(EXTRA_PARAMS) $(DEBUG_LEVEL_OPT) $(RFLAGS) -Wno-error=implicit-fallthrough -Wno-error=bool-operation -fomit-frame-pointer -falign-functions=16 -Werror -Wno-error=pointer-sign -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=sign-compare -Wno-error=unused-parameter -Wno-error=missing-field-initializers -Werror=type-limits -Wno-error=strict-aliasing -Wno-error=attributes
endif

# Configure the linker script to build the bootloader, not the firmware
USE_OPT += -Wl,--defsym=IS_BOOTLOADER=1

# C specific options here (added to USE_OPT).
ifeq ($(USE_COPT),)
  USE_COPT = -fgnu89-inline -std=gnu99 -Werror-implicit-function-declaration
endif

# C++ specific options here (added to USE_OPT).
ifeq ($(USE_CPPOPT),)
  # constexpr float expf_taylor_impl probably needs just c++14 but why not go with 17?
  USE_CPPOPT = -std=c++20 -Wno-register -fno-rtti -fno-exceptions -fno-use-cxa-atexit -Werror=write-strings -Werror=type-limits -Winvalid-pch
  # gcc-10 c++ 20 depricated uses of volatile errors
  USE_CPPOPT += -Wno-deprecated
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
ifeq ($(USE_SMART_BUILD),)
  USE_SMART_BUILD = no
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

#
# Architecture or project specific options
##############################################################################

##############################################################################
# Project, sources and paths
#

# Define project name here
PROJECT = fome_bl
PROJECT_DIR = ..
GENERATED_DIR = $(PROJECT_DIR)/generated

# Configure precompiled header
PCH_DIR = $(PROJECT_DIR)/pch
PCHSRC = $(PCH_DIR)/pch.h
PCHSUB = bootloader

# Imported source files and paths
CHIBIOS = $(PROJECT_DIR)/ChibiOS
# todo: looks like 'CHIBIOS_CONTRIB' path is universal shall we defined it only once?
CHIBIOS_CONTRIB = $(CHIBIOS)/../ChibiOS-Contrib

# Configure libfirmware Paths/Includes
RUSEFI_LIB = $(PROJECT_DIR)/libfirmware
include $(RUSEFI_LIB)/util/util.mk
include $(RUSEFI_LIB)/pt2001/pt2001.mk

RULESPATH = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk
RULESFILE = $(RULESPATH)/rules.mk

include $(PROJECT_DIR)/rusefi.mk

CONFIG = $(PROJECT_DIR)/config

# Include various ChibiOS mk files
# Licensing files.
include $(CHIBIOS)/os/license/license.mk
# Startup files.
include $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk/$(CPU_STARTUP)
# HAL-OSAL files (optional).
include $(CHIBIOS_CONTRIB)/os/hal/hal.mk
include $(CPU_PLATFORM)
include $(CHIBIOS)/os/hal/osal/rt-nil/osal.mk
# RTOS files (optional).
include $(CHIBIOS)/os/rt/rt.mk
include $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/mk/port_v7m.mk
include $(CHIBIOS)/os/various/cpp_wrappers/chcpp.mk
# EX files (optional).
include $(CHIBIOS)/os/hal/lib/streams/streams.mk

BOARDINC = $(BOARD_DIR)
include $(BOARD_DIR)/board.mk
include $(PROJECT_DIR)/init/init.mk
include $(PROJECT_DIR)/util/util.mk

include $(PROJECT_DIR)/controllers/controllers.mk
include $(PROJECT_DIR)/hw_layer/$(CPU_HWLAYER)/hw_ports.mk
include $(PROJECT_DIR)/hw_layer/drivers/drivers.mk

# C sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CSRC =  $(ALLCSRC) \
	$(BOARDSRC) \
	$(HW_LAYER_EMS) \
	$(HW_LAYER_DRIVERS_CORE) \
	$(PROJECT_DIR)/hw_layer/openblt/shared_params.c \
	$(PROJECT_DIR)/hw_layer/main_hardfault.c \
	$(PROJECT_DIR)/ext/openblt/Target/Source/backdoor.c \
	$(PROJECT_DIR)/ext/openblt/Target/Source/boot.c \
	$(PROJECT_DIR)/ext/openblt/Target/Source/com.c \
	$(PROJECT_DIR)/ext/openblt/Target/Source/xcp.c \
	$(PROJECT_DIR)/bootloader/openblt_chibios/nvm.c \
	$(PROJECT_DIR)/hw_layer/openblt/hooks.c \

# C++ sources that can be compiled in ARM or THUMB mode depending on the global
# todo: reduce code duplication with primary Makefile!!!
# setting.
CPPSRC = $(ALLCPPSRC) \
	$(HW_LAYER_EMS_CPP) \
	$(BOARDCPPSRC) \
	$(PROJECT_DIR)/util/efilib.cpp \
	$(PROJECT_DIR)/hw_layer/pin_repository.cpp \
	$(RUSEFI_LIB_CPP) \
	$(PROJECT_DIR)/bootloader/openblt_chibios/openblt_chibios.cpp \
	$(PROJECT_DIR)/bootloader/openblt_chibios/openblt_flash.cpp \
	$(PROJECT_DIR)/bootloader/openblt_chibios/openblt_usb.cpp \
	bootloader_stubs.cpp \
	bootloader_main.cpp

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
# List ASM source files here
ASMXSRC = $(STARTUPASM) $(PORTASM) $(OSALASM) \
	$(RUSEFIASM) \
	$(PROJECT_DIR)/hw_layer/main_hardfault_asm.S

INCDIR = $(ALLINC) \
	$(PCH_DIR) \
	.. \
	$(BOARDINC) \
	$(CHIBIOS)/os/various \
	$(CHIBIOS)/os/ex/ST \
	$(CHIBIOS)/os/hal/lib/peripherals/sensors \
	$(CONFIG)/engines \
	$(CONFIG) \
	$(CONFDIR) \
	$(PROJECT_DIR)/ext \
	$(PROJECT_DIR)/ext_algo \
	$(GENERATED_DIR) \
	$(UTIL_INC) \
	$(PROJECT_DIR)/console_util \
	$(PROJECT_DIR)/console \
	$(PROJECT_DIR)/console/binary \
	$(PROJECT_DIR)/console/binary_log \
	$(PROJECT_DIR)/console/fl_binary \
	$(PROJECT_DIR)/hw_layer \
	$(PROJECT_DIR)/hw_layer/digital_input \
	$(PROJECT_DIR)/hw_layer/adc \
	$(PROJECT_DIR)/hw_layer/mass_storage \
	$(PROJECT_DIR)/hw_layer/serial_over_usb \
	$(PROJECT_DIR)/hw_layer/algo \
	$(PROJECT_DIR)/hw_layer/sensors \
	$(PROJECT_DIR)/controllers/sensors/impl \
	$(PROJECT_DIR)/controllers/sensors/core \
	$(PROJECT_DIR)/hw_layer/mass_storage \
	$(PROJECT_DIR)/hw_layer/ports \
	$(PROJECT_DIR)/hw_layer/$(CPU_HWLAYER) \
	$(HW_INC) \
	$(HW_LAYER_DRIVERS_INC) \
	$(PROJECT_DIR)/development \
	$(PROJECT_DIR)/development/hw_layer \
	$(PROJECT_DIR)/development/test \
	$(CONTROLLERS_INC) \
	$(PROJECT_DIR)/controllers/sensors \
	$(PROJECT_DIR)/init \
	$(RUSEFI_LIB_INC) \
	$(BOARDS_DIR) \
	$(PROJECT_DIR)/hw_layer/openblt \
	$(PROJECT_DIR)/ext/openblt/Target/Source \
	$(PROJECT_DIR)/bootloader/openblt_chibios \
	config

BUILDDIR=blbuild

#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#

MCU  = cortex-m4

include $(PROJECT_DIR)/use_arm_gcc.mk

CC   = $(TRGT)gcc
CPPC = $(TRGT)g++
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
include $(PROJECT_DIR)/rusefi_pch.mk
include $(PROJECT_DIR)/fome_generated.mk
include $(PROJECT_DIR)/gitversion.mk
