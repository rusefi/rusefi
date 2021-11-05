#|--------------------------------------------------------------------------------------|
#| Collect project files                                                                |
#|--------------------------------------------------------------------------------------|
# Collect all application files in the current directory and its subdirectories, but 
# exclude flash-layout.c as this one is directly included in a source file, when used.
PROJ_FILES += $(filter-out $(OPENBLT_BOARD_DIR)/flash_layout.c, $(call rwildcard, $(OPENBLT_BOARD_DIR), *.c *.h *.s))

#|--------------------------------------------------------------------------------------|
#| Specific options for toolchain binaries                                              |
#|--------------------------------------------------------------------------------------|
BRDFLAGS    = -DSTM32F767xx -DHSE_VALUE=25000000

#|--------------------------------------------------------------------------------------|
#| Specify library files                                                                |
#|--------------------------------------------------------------------------------------|
LIBS = 

