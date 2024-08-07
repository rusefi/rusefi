# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp
DDEFS += -DEFI_MAIN_RELAY_CONTROL=TRUE

# Add them all together
DDEFS += -DFIRMWARE_ID=\"hellen81\"
#DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE

# we need fast ADC for software trigger detector
#DDEFS += -DADC_FAST_DEVICE=ADCD1 -DADC_SLOW_DEVICE=ADCD3 -DSTM32_ADC_USE_ADC3=TRUE
#DDEFS += -DADC_SLOW_DEVICE=ADCD3 -DSTM32_ADC_USE_ADC3=TRUE

# we probably do not need this DDEFS += -DADC_FAST_DEVICE=ADCD1


# implementation not in master as of today DDEFS += -DEFI_FASTER_UNIFORM_ADC=TRUE
# for analog VR we want to access un-averaged most raw data
# DDEFS += -DADC_BUF_DEPTH_FAST=1
# DDEFS += -DADC_BUF_NUM_AVG=1


SHORT_BOARD_NAME = hellen81

include $(BOARDS_DIR)/hellen/hellen-common176.mk
