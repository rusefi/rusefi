DDEFS += -DADC_MUX_PIN=Gpio::F2
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE

ONBOARD_MEMS_TYPE=LIS2DH12

# older mcu-176 has custom LEDs while mm-176 matches mm-144
BOARDCPPSRC += $(BOARDS_DIR)/hellen/hellen_leds_144.cpp
include $(BOARDS_DIR)/hellen/hellen-common176.mk
