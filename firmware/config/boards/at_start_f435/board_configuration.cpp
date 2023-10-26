#include "pch.h"

Gpio getCommsLedPin() {
	return Gpio::D15;	/* LED4 - green */
}

Gpio getRunningLedPin() {
	return Gpio::D14;   /* LED3 - yellow */
}

Gpio getWarningLedPin() {
	// this board has no warning led
	return Gpio::Unassigned;
}

void setBoardConfigOverrides() {
}
