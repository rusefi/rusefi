#|--------------------------------------------------------------------------------------|
#| Collect project files                                                                |
#|--------------------------------------------------------------------------------------|
# Collect all application files in the current directory and its subdirectories, but 
# exclude flash-layout.c as this one is directly included in a source file, when used.
PROJ_FILES += $(filter-out $(OPENBLT_PORT_DIR)/flash_layout.c, $(call rwildcard, $(OPENBLT_PORT_DIR), *.c *.h *.s))
# reuse ST32F4xx HAL and CMSIS from one of OpenBLT exaples to avoid having copy in RusEFI git
PROJ_FILES += $(filter-out uip, $(call rwildcard, $(OPENBLT_TRGT_DIR)/Demo/ARMCM4_STM32F4_Nucleo_F429ZI_GCC/Boot/lib/CMSIS/, *.c *.h *.s))
PROJ_FILES += $(filter-out uip, $(call rwildcard, $(OPENBLT_TRGT_DIR)/Demo/ARMCM4_STM32F4_Nucleo_F429ZI_GCC/Boot/lib/STM32F4xx_HAL_Driver/, *.c *.h *.s))

#|--------------------------------------------------------------------------------------|
#| Options for toolchain binaries                                                       |
#|--------------------------------------------------------------------------------------|
BRDFLAGS     = -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER
