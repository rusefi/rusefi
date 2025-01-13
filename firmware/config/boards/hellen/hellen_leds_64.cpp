// H64_LED1_RED usage see hellen-common64.mk

Gpio getCommsLedPin() {
	return Gpio::H64_LED2_BLUE;
}

Gpio getWarningLedPin() {
	// this board has no warning led
	return Gpio::Unassigned;
}

Gpio getRunningLedPin() {
	// this board has no running led
	return Gpio::Unassigned;
}

