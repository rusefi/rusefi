#|--------------------------------------------------------------------------------------|
#| Collect port files                                                                |
#|--------------------------------------------------------------------------------------|
# Collect all application files in the current directory and its subdirectories, but 
# exclude flash-layout.c as this one is directly included in a source file, when used.
PROJ_FILES += $(filter-out $(OPENBLT_PORT_DIR)/flash_layout.c, $(call rwildcard, $(OPENBLT_PORT_DIR), *.c *.h *.s))
# reuse ST32F7xx HAL and CMSIS from one of OpenBLT exaples to avoid having copy in RusEFI git
PROJ_FILES += $(filter-out uip, $(call rwildcard, $(OPENBLT_TRGT_DIR)/Demo/ARMCM7_STM32F7_Nucleo_F767ZI_GCC/Boot/lib/CMSIS/, *.c *.h *.s))
PROJ_FILES += $(filter-out uip, $(call rwildcard, $(OPENBLT_TRGT_DIR)/Demo/ARMCM7_STM32F7_Nucleo_F767ZI_GCC/Boot/lib/STM32F7xx_HAL_Driver/, *.c *.h *.s))
PROJ_FILES += $(wildcard $(OPENBLT_TRGT_DIR)/Demo/ARMCM7_STM32F7_Nucleo_F746ZG_GCC/Boot/lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_can.c)
PROJ_FILES += $(wildcard $(OPENBLT_TRGT_DIR)/Demo/ARMCM7_STM32F7_Nucleo_F746ZG_GCC/Boot/lib/STM32F7xx_HAL_Driver/Inc/stm32f7xx_hal_can.h)

#|--------------------------------------------------------------------------------------|
#| Specific options for toolchain binaries                                              |
#|--------------------------------------------------------------------------------------|
PORTFLAGS   = -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER
