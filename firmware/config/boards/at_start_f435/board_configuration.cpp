#include "pch.h"

void setBoardConfigOverrides() {
	// PD13 is error LED, configured in board.mk
	engineConfiguration->communicationLedPin = Gpio::D14;
	engineConfiguration->runningLedPin = Gpio::D15;

	// Board only has 3 LEDs
	engineConfiguration->warningLedPin = Gpio::Unassigned;
}
