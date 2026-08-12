// file hellen_leds_100.cpp

Gpio getCommsLedPin() {
	return Gpio::MM100_LED3_BLUE;
}

Gpio getRunningLedPin() {
	return Gpio::MM100_LED2_GREEN;
}

Gpio getWarningLedPin() {
	return Gpio::MM100_LED4_YELLOW;
}
