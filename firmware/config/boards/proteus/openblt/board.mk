#|--------------------------------------------------------------------------------------|
#| Collect project files                                                                |
#|--------------------------------------------------------------------------------------|
# Collect all application files in the current directory and its subdirectories, but 
# exclude flash-layout.c as this one is directly included in a source file, when used.
PROJ_FILES += $(filter-out $(OPENBLT_BOARD_DIR)/flash_layout.c, $(call rwildcard, $(OPENBLT_BOARD_DIR), *.c *.h *.s))

#|--------------------------------------------------------------------------------------|
#| Options for toolchain binaries                                                       |
#|--------------------------------------------------------------------------------------|
ifeq ($(PROJECT_CPU),ARCH_STM32F4)
	BRDFLAGS     = -DSTM32F429xx
else ifeq ($(PROJECT_CPU),ARCH_STM32F7)
	#TODO: check what CPU is on proteus f7
	BRDFLAGS     = -DSTM32F767xx
else ifeq ($(PROJECT_CPU),ARCH_STM32H7)
	#TODO: check what CPU is on proteus h7
	BRDFLAGS     = -DSTM32H743xx
endif

BRDFLAGS    += -DHSE_VALUE=8000000

#|--------------------------------------------------------------------------------------|
#| Specify library files                                                                |
#|--------------------------------------------------------------------------------------|
LIBS = 
