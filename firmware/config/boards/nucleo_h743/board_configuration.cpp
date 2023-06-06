#include "pch.h"

// PB14 is error LED, configured in board.mk
// Board only has 3 LEDs, so no warning LED
Gpio getCommsLedPin() {
	return Gpio::E1;
}

Gpio getRunningLedPin() {
	return Gpio::B0;
}

void preHalInit() {
	efiSetPadMode("Ethernet",  Gpio::A1, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("Ethernet",  Gpio::A2, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("Ethernet",  Gpio::A7, PAL_MODE_ALTERNATE(0xb));

	efiSetPadMode("Ethernet", Gpio::B13, PAL_MODE_ALTERNATE(0xb));

	efiSetPadMode("Ethernet",  Gpio::C1, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("Ethernet",  Gpio::C4, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("Ethernet",  Gpio::C5, PAL_MODE_ALTERNATE(0xb));

	efiSetPadMode("Ethernet", Gpio::G11, PAL_MODE_ALTERNATE(0xb));
	efiSetPadMode("Ethernet", Gpio::G13, PAL_MODE_ALTERNATE(0xb));
}
