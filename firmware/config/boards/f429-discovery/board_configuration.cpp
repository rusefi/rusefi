#include "pch.h"

/**
 * @brief Board-specific initialization code.
 */
void boardInit(void) {
}

Gpio getWarningLedPin() {
	// this board has no warning led
	return Gpio::Unassigned;
}

Gpio getCommsLedPin() {
	return Gpio::Unassigned;
}

Gpio getRunningLedPin() {
	// LD3 - green
	return Gpio::G13;
}

void setup_custom_board_overrides() {
}
