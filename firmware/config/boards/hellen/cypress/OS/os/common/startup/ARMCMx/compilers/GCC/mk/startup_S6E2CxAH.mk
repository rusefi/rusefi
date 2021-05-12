
ifeq ($(CYPRESS_CONTRIB),)
  CYPRESS_CONTRIB = $(CHIBIOS_CONTRIB)
endif

# List of the ChibiOS generic KE1xF startup and CMSIS files.
STARTUPSRC = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/crt1.c

STARTUPASM = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/crt0_v7m.S \
             $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/vectors.S

STARTUPINC = $(CYPRESS_CONTRIB)/os/common/startup/ARMCMx/devices/S6E2CxAH \
             $(CYPRESS_CONTRIB)/os/common/ext/CMSIS/S6E2CxAH \
             $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC \
             $(CHIBIOS)/os/common/ext/CMSIS/include \
             $(CHIBIOS)/os/common/ext/ARM/CMSIS/Core/Include

STARTUPLD_CYPRESS  = $(CYPRESS_CONTRIB)/os/common/startup/ARMCMx/compilers/GCC/ld
STARTUPLD = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/ld

# Shared variables
ALLXASMSRC += $(STARTUPASM)
ALLCSRC    += $(STARTUPSRC)
ALLINC     += $(STARTUPINC)
