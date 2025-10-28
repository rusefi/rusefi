Gpio getCommsLedPin() {
	return Gpio::MM100_LED3_BLUE;
}

Gpio getRunningLedPin() {
	return Gpio::MM100_LED2_GREEN;
}

// fun fact: the red LED for critical error is defined via LED_CRITICAL_ERROR_BRAIN_PIN
Gpio getWarningLedPin() {
	return Gpio::MM100_LED4_YELLOW;
}
