# hellen-common.mk

# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC += $(BOARDS_DIR)/hellen/hellen_common.cpp

ifeq (,$(findstring EFI_BOOTLOADER,$(DDEFS)))
BOARDCPPSRC += $(BOARDS_DIR)/hellen/hellen_board_id.cpp
endif

BOARDINC += $(BOARDS_DIR)/hellen

DDEFS += -DLED_PIN_MODE=OM_INVERTED

PRIMARY_COMMUNICATION_PORT_USART2=-DEFI_CONSOLE_TX_BRAIN_PIN=Gpio::D6 -DEFI_CONSOLE_RX_BRAIN_PIN=Gpio::D5 -DTS_PRIMARY_UxART_PORT=UARTD2 -DSTM32_UART_USE_USART2=1

# We are running on Hellen-One hardware!
DDEFS += -DHW_HELLEN=1

ifeq ($(ONBOARD_MEMS_TYPE),LIS2DH12)
 # Same spi is used for SD and Accelerometer
 DDEFS += -DMMC_USE_MUTUAL_EXCLUSION=TRUE

 # This board has some Accelerometer onboard
 DDEFS += -DEFI_ONBOARD_MEMS=TRUE
 EFI_ONBOARD_MEMS_LIS2DH12 = yes
 DDEFS += -DLSM303AGR_USE_SPI=TRUE -DLSM303AGR_USE_I2C=FALSE -DLSM303AGR_SHARED_SPI=TRUE
endif
