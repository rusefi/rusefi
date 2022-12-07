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

# We use HSI (internal) on proteus. We define HSE (external) only to have compiler happy.
BRDFLAGS    += -DHSE_VALUE=8000000

#|--------------------------------------------------------------------------------------|
#| Specify library files                                                                |
#|--------------------------------------------------------------------------------------|
LIBS = 

BRDFLAGS += -DSTATUS_LED_PORT=GPIOE -DSTATUS_LED_PIN=GPIO_PIN_3

# Frequency of the external crystal oscillator
BRDFLAGS += -DBOOT_CPU_XTAL_SPEED_KHZ=8000
BRDFLAGS += -DBOOT_CPU_SYSTEM_SPEED_KHZ=168000
# Select the desired UART peripheral as a zero based index.
BRDFLAGS += -DBOOT_COM_RS232_CHANNEL_INDEX=2
# \brief Select the desired CAN peripheral as a zero based index.
BRDFLAGS += -DBOOT_COM_CAN_CHANNEL_INDEX=0

PROJ_FILES += $(PROJECT_DIR)/hw_layer/openblt/main_internal_osc.c
