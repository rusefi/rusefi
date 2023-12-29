#|--------------------------------------------------------------------------------------|
#| Collect port files                                                                |
#|--------------------------------------------------------------------------------------|
# Collect all application files in the current directory and its subdirectories, but 
# exclude flash-layout.c as this one is directly included in a source file, when used.
#PROJ_FILES += $(filter-out $(OPENBLT_PORT_DIR)/flash_layout.c, $(call rwildcard, $(OPENBLT_PORT_DIR), *.c *.h *.s))
# reuse ST32F7xx HAL and CMSIS from one of OpenBLT examples to avoid having copy in rusEFI git
OPENBLT_PORT_DIR = $(OPENBLT_TRGT_DIR)/Source/ARMCM7_STM32H7
OPENBLT_PORT_FILES += $(filter-out uip, $(call wildcard, $(OPENBLT_PORT_DIR)/*.c $(OPENBLT_PORT_DIR)/*.h $(OPENBLT_PORT_DIR)/*.s))
OPENBLT_PORT_DIR_GCC = $(OPENBLT_PORT_DIR)/GCC
OPENBLT_PORT_FILES += $(filter-out uip, $(call wildcard, $(OPENBLT_PORT_DIR_GCC)/*.c $(OPENBLT_PORT_DIR_GCC)/*.h $(OPENBLT_PORT_DIR_GCC)/*.s))
#|--------------------------------------------------------------------------------------|
#| Specific options for toolchain binaries                                              |
#|--------------------------------------------------------------------------------------|
#PORTFLAGS   = -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER
