#|--------------------------------------------------------------------------------------|
#| Collect project files                                                                |
#|--------------------------------------------------------------------------------------|
# Collect all application files in the current directory and its subdirectories, but 
# exclude flash-layout.c as this one is directly included in a source file, when used.
PROJ_FILES  = $(filter-out $(OPENBLT_BOARD_DIR)/flash_layout.c, $(call rwildcard, $(OPENBLT_BOARD_DIR), *.c *.h *.s))
# reuse ST32F4xx HAL and CMSIS from one of OpenBLT exaples to avoid having copy in RusEFI git
PROJ_FILES += $(filter-out uip, $(call rwildcard, $(OPENBLT_TRGT_DIR)/Demo/ARMCM4_STM32F4_Nucleo_F429ZI_GCC/Boot/lib/CMSIS/, *.c *.h *.s))
PROJ_FILES += $(filter-out uip, $(call rwildcard, $(OPENBLT_TRGT_DIR)/Demo/ARMCM4_STM32F4_Nucleo_F429ZI_GCC/Boot/lib/STM32F4xx_HAL_Driver/, *.c *.h *.s))
# Collect bootloader port files
PROJ_FILES += $(wildcard $(OPENBLT_TRGT_DIR)/Source/ARMCM4_STM32F4/*.c)
PROJ_FILES += $(wildcard $(OPENBLT_TRGT_DIR)/Source/ARMCM4_STM32F4/*.h)
# Collect bootloader port compiler specific files
PROJ_FILES += $(wildcard $(OPENBLT_TRGT_DIR)/Source/ARMCM4_STM32F4/GCC/*.c)
PROJ_FILES += $(wildcard $(OPENBLT_TRGT_DIR)/Source/ARMCM4_STM32F4/GCC/*.h)

#|--------------------------------------------------------------------------------------|
#| Options for toolchain binaries                                                       |
#|--------------------------------------------------------------------------------------|
STDFLAGS    = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fno-strict-aliasing
STDFLAGS   += -fdata-sections -ffunction-sections -Wall -g3
OPTFLAGS    = -Os
CFLAGS      = $(STDFLAGS) $(OPTFLAGS)
CFLAGS     += -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -DSTM32F429xx
CFLAGS     += $(INC_PATH)
AFLAGS      = $(CFLAGS)
LFLAGS      = $(STDFLAGS) $(OPTFLAGS)
LFLAGS     += -Wl,-script="$(OPENBLT_BOARD_DIR)/STM32F429ZITx_FLASH.ld" -Wl,-Map=$(BIN_PATH)/$(PROJ_NAME).map
LFLAGS     += -specs=nano.specs -Wl,--gc-sections $(LIB_PATH)
OFLAGS      = -O srec
ODFLAGS     = -x
SZFLAGS     = -B -d
RMFLAGS     = -f

#|--------------------------------------------------------------------------------------|
#| Specify library files                                                                |
#|--------------------------------------------------------------------------------------|
LIBS = 
