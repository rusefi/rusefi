#|--------------------------------------------------------------------------------------|
#| Collect port files                                                                |
#|--------------------------------------------------------------------------------------|
# Collect all application files in the current directory and its subdirectories, but 
# exclude flash-layout.c as this one is directly included in a source file, when used.
#PROJ_FILES += $(filter-out $(OPENBLT_PORT_DIR)/flash_layout.c, $(call rwildcard, $(OPENBLT_PORT_DIR), *.c *.h *.s))
# reuse ST32F7xx HAL and CMSIS from one of OpenBLT examples to avoid having copy in rusEFI git
OPENBLT_PORT_FILES += $(filter-out uip, $(call rwildcard, $(OPENBLT_TRGT_DIR)/Source/ARMCM4_STM32H7/, *.c *.h *.s))
OPENBLT_PORT_FILES += $(filter-out uip, $(call rwildcard, $(OPENBLT_TRGT_DIR)/Source/ARMCM4_STM32H7/GCC/, *.c *.h *.s))
#|--------------------------------------------------------------------------------------|
#| Specific options for toolchain binaries                                              |
#|--------------------------------------------------------------------------------------|
#PORTFLAGS   = -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER
