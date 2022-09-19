ifeq ($(CYPRESS_CONTRIB),)
  CYPRESS_CONTRIB = $(CHIBIOS_CONTRIB)
endif

ifeq ($(PDL_DIR),)
PDL_DIR = $(CYPRESS_CONTRIB)/os/hal/ports/Cypress/PDL
endif

USBSRC = $(PDL_DIR)/midware/usb/device/UsbDeviceCdcCom.c \
	$(PDL_DIR)/driver/usb/usb.c \
	$(PDL_DIR)/driver/usb/usbdevice.c \
	$(PDL_DIR)/driver/usb/usbethernetclock.c
	
PDLSRC := $(PDL_DIR)/driver/pdl.c \
	$(PDL_DIR)/driver/interrupts_fm4_type_b.c \
	$(PDL_DIR)/driver/adc/adc.c \
	$(PDL_DIR)/driver/bt/bt.c \
	$(PDL_DIR)/driver/can/canfd.c \
	$(PDL_DIR)/driver/can/can_pre.c \
	$(PDL_DIR)/driver/exint/exint.c \
	$(PDL_DIR)/driver/flash/dualflash.c \
	$(PDL_DIR)/driver/flash/mainflash.c \
	$(PDL_DIR)/driver/mfs/mfs.c \
	$(USBSRC)

PLATFORMSRC_CONTRIB := ${CHIBIOS}/os/hal/ports/common/ARMCMx/nvic.c \
                       ${CYPRESS_CONTRIB}/os/hal/ports/Cypress/S6E2CxAH/hal_lld.c \
                       ${CYPRESS_CONTRIB}/os/hal/ports/Cypress/S6E2CxAH/system_s6e2c5.c \
                       ${CYPRESS_CONTRIB}/os/hal/ports/Cypress/LLD/PITv2/hal_st_lld.c \
                       $(PDLSRC)
                       
PLATFORMINC_CONTRIB := ${CHIBIOS}/os/hal/ports/common/ARMCMx \
                       ${CYPRESS_CONTRIB}/os/hal/ports/Cypress/LLD \
                       ${CYPRESS_CONTRIB}/os/hal/ports/Cypress/S6E2CxAH \
                       $(PDL_DIR)

include ${CYPRESS_CONTRIB}/os/hal/ports/Cypress/LLD/GPIOv2/driver.mk
include ${CYPRESS_CONTRIB}/os/hal/ports/Cypress/LLD/SPIv2/driver.mk
include ${CYPRESS_CONTRIB}/os/hal/ports/Cypress/LLD/ADCv2/driver.mk
include ${CYPRESS_CONTRIB}/os/hal/ports/Cypress/LLD/PITv2/driver.mk
include ${CYPRESS_CONTRIB}/os/hal/ports/Cypress/LLD/CANv2/driver.mk

# Shared variables
ALLCSRC += $(PLATFORMSRC_CONTRIB)
ALLINC  += $(PLATFORMINC_CONTRIB)
