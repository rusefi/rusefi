// file hellen_leds_100.cpp

#include "pch.h"
#include "hellen_mm100_meta.h"

Gpio __attribute__((weak)) getCommsLedPin() {
	return Gpio::MM100_LED3_BLUE;
}

Gpio __attribute__((weak)) getRunningLedPin() {
	return Gpio::MM100_LED2_GREEN;
}

Gpio __attribute__((weak)) getWarningLedPin() {
	return Gpio::MM100_LED4_YELLOW;
}
