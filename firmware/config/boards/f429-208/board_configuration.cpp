#include "pch.h"

/**
 * @brief Board-specific initialization code.
 */
void boardInit() {
}

Gpio getRunningLedPin() {
	return Gpio::G13; /* LD3 - green */
}

/**
 * @brief   Board-specific configuration defaults.

 */
void setBoardDefaultConfiguration() {
}
