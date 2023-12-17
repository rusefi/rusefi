# hellen-common.mk

# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC += $(BOARDS_DIR)/hellen/hellen_common.cpp \
    $(BOARDS_DIR)/hellen/hellen_board_id.cpp

BOARDINC += $(BOARDS_DIR)/hellen

DDEFS += -DLED_PIN_MODE=OM_INVERTED

# We are running on Hellen-One hardware!
DDEFS += -DHW_HELLEN=1

ifeq ($(RAM_UNUSED_SIZE),)
    DDEFS += -DRAM_UNUSED_SIZE=100
endif

ifeq ($(ONBOARD_MEMS_TYPE),LIS2DH12)
 # Same spi is used for SD and Accelerometer
 DDEFS += -DMMC_USE_MUTUAL_EXCLUSION=TRUE

 # This board has some Accelerometer onboard
 DDEFS += -DEFI_ONBOARD_MEMS=TRUE
 EFI_ONBOARD_MEMS_LIS2DH12 = yes
 DDEFS += -DLSM303AGR_USE_SPI=TRUE -DLSM303AGR_USE_I2C=FALSE -DLSM303AGR_SHARED_SPI=TRUE
endif
