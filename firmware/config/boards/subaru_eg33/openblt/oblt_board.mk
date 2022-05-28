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

BRDFLAGS += -DSTATUS_LED_PORT=GPIOG -DSTATUS_LED_PIN=GPIO_PIN_8

# Frequency of the external crystal oscillator
BRDFLAGS += -DBOOT_CPU_XTAL_SPEED_KHZ=25000
BRDFLAGS += -DBOOT_CPU_SYSTEM_SPEED_KHZ=216000
BRDFLAGS += -DBOOT_COM_RS232_CHANNEL_INDEX=0
# \brief Select the desired CAN peripheral as a zero based index.
BRDFLAGS += -DBOOT_COM_CAN_CHANNEL_INDEX=0