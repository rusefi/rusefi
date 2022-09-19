# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARDS_DIR)/hellen/hellen81/board_configuration.cpp
BOARDINC = $(BOARDS_DIR)/hellen/hellen81

DDEFS += -DEFI_MAIN_RELAY_CONTROL=TRUE

# Disable serial ports on this board as UART3 causes a DMA conflict with the SD card
DDEFS += -DTS_NO_PRIMARY

# Add them all together
DDEFS += -DFIRMWARE_ID=\"hellen81\"
#DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE
DDEFS += -DHAL_TRIGGER_USE_ADC=TRUE

# we need fast ADC for software trigger detector
#DDEFS += -DADC_FAST_DEVICE=ADCD1 -DADC_SLOW_DEVICE=ADCD3 -DSTM32_ADC_USE_ADC3=TRUE
#DDEFS += -DADC_SLOW_DEVICE=ADCD3 -DSTM32_ADC_USE_ADC3=TRUE

# we probably do not need this DDEFS += -DADC_FAST_DEVICE=ADCD1


# implementation not in master as of today DDEFS += -DEFI_FASTER_UNIFORM_ADC=TRUE
# for analog VR we want to access un-averaged most raw data
# DDEFS += -DADC_BUF_DEPTH_FAST=1
# DDEFS += -DADC_BUF_NUM_AVG=1


DDEFS += -DSHORT_BOARD_NAME=hellen81

include $(BOARDS_DIR)/hellen/hellen-common176.mk
