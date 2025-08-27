# hellen-common176.mk

DDEFS += $(LED_CRITICAL_ERROR_BRAIN_PIN)

DDEFS += -DHELLEN_BOARD_MM176=TRUE

DDEFS += -DHELLEN_BOARD_ID_PIN_1=Gpio::F0 -DHELLEN_BOARD_ID_PIN_2=Gpio::F1

# older mcu-176 has custom LEDs while mm-176 matches mm-144
include $(BOARDS_DIR)/hellen/hellen-common.mk
