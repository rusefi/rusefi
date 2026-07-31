BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp

# Hellen platform common bits: HW_HELLEN=1, hellen_common.cpp, board-id
# (inert here - the premium module has no board-id pads/pins defined), and
# LED_PIN_MODE=OM_INVERTED which matches this module's LED wiring
# (V33 -> resistor -> LED -> MCU pin, the pin sinks current)
include $(BOARDS_DIR)/hellen/hellen-common.mk

DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::MMP176_LED1_RED

# third bxCAN peripheral for the third on-module transceiver
# (EFI_CAN_BUS_COUNT=3 comes from prepend.txt via the generated config)
DDEFS += -DSTM32_CAN_USE_CAN3=TRUE

# AIN1..6 analog mux select = PH15 (MMP176_MUX_CONTROL; raw pin here because
# the ADC driver TU does not see hellen meta names). The slow ADC samples
# channels 0-15 with this pin LOW (AIN1/AIN4/AIN6) and 16-31 with it HIGH
# (AIN2/AIN3/AIN5), see stm32_adc_v2.cpp
DDEFS += -DADC_MUX_PIN=Gpio::H15

# on-module LAN8720A RMII PHY: TunerStudio over TCP (ethernet_console + lwIP).
# RMII pin AF and PHY nRST release happen in board preHalInit
EFI_ETHERNET = yes
DDEFS += -DBOARD_PHY_ID=MII_LAN8720_ID

# on-module KLMAG1JETD 16GB eMMC on 8-bit SDMMC2 (pin map in
# hellen_premium176_meta.h). It rides the standard SD-card stack:
# EFI_SDC_DEVICE flips HAL_USE_SDC on in rusefi_halconf.h, and the shared
# F7 mcuconf already enables SDMMC2. The mounted block device is exposed
# over USB MSD by the regular SD/MSD machinery.
DDEFS += -DEFI_SDC_DEVICE=SDCD2
# ChibiOS MMC/eMMC init path (CMD1 + CMD6 SWITCH), off by default
DDEFS += -DSDC_MMC_SUPPORT=TRUE
# all eight data lines are wired
DDEFS += -DRE_SDC_MODE=SDC_MODE_8BIT

# USB FS device on module pads USBM/USBP (PA11/PA12, AF10) - same as the
# STM32F7 port defaults, stated explicitly; module has no USB ID pad, and
# VBUS sensing is ignored (BOARD_OTG_NOVBUSSENS in the port board.h)
DDEFS += -DEFI_USB_SERIAL_DM=Gpio::A11
DDEFS += -DEFI_USB_SERIAL_DP=Gpio::A12

DDEFS += -DHW_HELLEN_SKIP_BOARD_TYPE=TRUE
DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_PREMIUM_Q_TEST
