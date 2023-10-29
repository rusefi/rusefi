#include "pch.h"

Gpio getCommsLedPin() {
	return Gpio::D15;	/* LED4 - green */
}

Gpio getRunningLedPin() {
	return Gpio::D14;   /* LED3 - yellow */
}

void setBoardConfigOverrides() {
}
