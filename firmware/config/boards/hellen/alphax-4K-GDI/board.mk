

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp

DDEFS += -DADC_MUX_PIN=Gpio::F2

DDEFS += -DEFI_MAIN_RELAY_CONTROL=TRUE
DDEFS += -DBOARD_MC33810_COUNT=1








DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE


include $(BOARDS_DIR)/hellen/hellen-common144.mk

