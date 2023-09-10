Gpio getCommsLedPin() {
	return Gpio::MM100_LED2;  // blue
}

Gpio getRunningLedPin() {
	return Gpio::MM100_LED3;  // green
}

Gpio getWarningLedPin() {
	return Gpio::MM100_LED4; // yellow
}
