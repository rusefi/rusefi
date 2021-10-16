
ifeq ($(KINETIS_CONTRIB),)
  KINETIS_CONTRIB = $(CHIBIOS_CONTRIB)
endif

# List of the ChibiOS generic KE1xF startup and CMSIS files.
STARTUPSRC = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/crt1.c

STARTUPASM = $(KINETIS_CONTRIB)/os/common/startup/ARMCMx/compilers/GCC/startup_ke1xf.S \
             $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/crt0_v7m.S \
             $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/vectors.S

STARTUPINC = $(KINETIS_CONTRIB)/os/common/startup/ARMCMx/devices/KE1xF \
             $(KINETIS_CONTRIB)/os/common/ext/CMSIS/KINETIS \
             $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC \
             $(CHIBIOS)/os/common/ext/CMSIS/include \
             $(CHIBIOS)/os/common/ext/ARM/CMSIS/Core/Include

STARTUPLD_KINETIS  = $(KINETIS_CONTRIB)/os/common/startup/ARMCMx/compilers/GCC/ld
STARTUPLD = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/ld

# Shared variables
ALLXASMSRC += $(STARTUPASM)
ALLCSRC    += $(STARTUPSRC)
ALLINC     += $(STARTUPINC)
