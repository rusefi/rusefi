# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp

BOARDINC  = $(BOARD_DIR)

# TLE8888 has it's own main relay control output, we are not involved
DDEFS += -DEFI_MAIN_RELAY_CONTROL=FALSE

# see also openblt/board.mk STATUS_LED
DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::E3

# *TEMPORARY* breaking TTL thus breaking Bluetooth for microRusEFI in order to enable SPI3 for SD card
# *TODO* need to give people the horrible choice between Bluetooth via TTL or SD card via SPI :( horrible choice
# PB10/PB11 uses UART3 peripheral and J12/J13 on MRE
# we also have PC10/PC11 exposed on J4 but that's same UART3
DDEFS += -DEFI_CONSOLE_TX_BRAIN_PIN=Gpio::B10 -DEFI_CONSOLE_RX_BRAIN_PIN=Gpio::B11

# on MRE 0.6.0 we have SD card on SPI2 which shared channel 3 with USART3
# todo: enable serial which would not DMA thus not conflict?
DDEFS += -DSTM32_UART_USE_USART3=FALSE -DHAL_USE_UART=FALSE
DDEFS += -DEFI_USE_UART_DMA=FALSE

# temp work-around only until June 3rd 2024
DDEFS += -DETB_INTERMITTENT_LIMIT=30000

DDEFS += -DEFI_AUX_VALVES=TRUE

# maybe a way to disable SPI2 privately
#DDEFS += -DSTM32_SPI_USE_SPI2=FALSE

DDEFS += -DBOARD_TLE8888_COUNT=1
DDEFS += -DEFI_MC33816=TRUE

DDEFS += -DFIRMWARE_ID=\"microRusEFI\"
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE
DDEFS += $(VAR_DEF_ENGINE_TYPE)

# F7 we are running out of flash?! only package goodies into F4
ifeq ($(PROJECT_CPU),ARCH_STM32F4)
    # This board can capture SENT
    DDEFS += -DEFI_SENT_SUPPORT=TRUE

    # This board has LIN/K-line interface
    DDEFS += -DEFI_KLINE=TRUE
endif

DDEFS += -DKLINE_SERIAL_DEVICE_RX=D9 -DKLINE_SERIAL_DEVICE_TX=D8
DDEFS += -DKLINE_SERIAL_DEVICE=SD3
DDEFS += -DSTM32_SERIAL_USE_USART3=TRUE

# We are running on microRusEFI hardware!
DDEFS += -DHW_MICRO_RUSEFI=1

ifeq ($(PROJECT_CPU),ARCH_STM32F7)
    DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_MRE_F7
else ifeq ($(PROJECT_CPU),ARCH_STM32F4)
    DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_MRE_F4
else
$(error Unsupported PROJECT_CPU [$(PROJECT_CPU)])
endif

ifeq ($(BOARD_HAS_EXT_FLASH),yes)
    # MRE has optional external SPI flash that uses ChibiOS MFS driver
    include $(PROJECT_DIR)/hw_layer/ports/stm32/use_higher_level_flash_api.mk
    include $(PROJECT_DIR)/hw_layer/drivers/flash/w25q/w25q_single_spi.mk
    DDEFS += -DEFI_STORAGE_SD=FALSE
    DDEFS += -DEFI_STORAGE_MFS_EXTERNAL=TRUE
    # Use same method to workaround DMA access to CCM memory where persistentState can be located
    DDEFS += -DSNOR_SPI_WORKAROUND_CACHE=TRUE
    # This board has SPI flash instead of SD connector
    DDEFS += -DEFI_FILE_LOGGING=FALSE
    BOARDCPPSRC += $(BOARD_DIR)/board_storage.cpp
endif

DDEFS += -DBOARD_SERIAL="\"000220000000000000000000\""
