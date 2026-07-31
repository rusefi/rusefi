/**
 * @file boards/hellen/premium-quick-test/board_configuration.cpp
 *
 * Bench bring-up board for the Hellen mega-mcu-premium 176-pin megamodule,
 * see hellen_premium176_meta.h
 */

#include "pch.h"
#include "hellen_meta.h"
#include "board_overrides.h"

// module LEDs sink current (V33 -> resistor -> LED -> MCU pin), see
// LED_PIN_MODE=OM_INVERTED in board.mk; red LED1 is the critical-error LED
Gpio getCommsLedPin() {
	return Gpio::MMP176_LED3_BLUE;
}

Gpio getRunningLedPin() {
	return Gpio::MMP176_LED2_GREEN;
}

Gpio getWarningLedPin() {
	return Gpio::MMP176_LED4_YELLOW;
}

static void premiumQuickTestDefaultConfiguration() {
	// on-module LPS22HB barometer, bit-banged I2C on the module SCL/SDA pads
	engineConfiguration->lps25BaroSensorScl = Gpio::MMP176_I2C_SCL;
	engineConfiguration->lps25BaroSensorSda = Gpio::MMP176_I2C_SDA;
}

void setup_custom_board_overrides() {
	custom_board_DefaultConfiguration = premiumQuickTestDefaultConfiguration;
}
