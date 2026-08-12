/**
 * @file board_configuration.cpp
 *
 * STM32F469I-DISCO user LEDs: LD1 green PG6, LD2 orange PD4, LD3 red PD5, LD4 blue PK3.
 * PK3 is not usable here: the generic STM32F4 HAL configuration does not enable GPIOK.
 */

#include "pch.h"
#include "board_overrides.h"

Gpio getCommsLedPin() {
	// LD2 orange
	return Gpio::D4;
}

Gpio getWarningLedPin() {
	// LD3 red
	return Gpio::D5;
}

Gpio getRunningLedPin() {
	// LD1 green
	return Gpio::G6;
}

void setup_custom_board_overrides() {
}
