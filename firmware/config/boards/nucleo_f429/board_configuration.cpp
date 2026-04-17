#include "pch.h"
#include "board_overrides.h"

// PB14 is error LED, configured in board.mk
// Board only has 3 LEDs, so no warning LED

Gpio getCommsLedPin() {
	return Gpio::B7;
}

Gpio getRunningLedPin() {
	return Gpio::B0;
}

Gpio getWarningLedPin() {
	// this board has no warning led
	return Gpio::Unassigned;
}

static void nucleo_f429_preHalInit() {
	/* TX_EN */
	efiSetPadMode("Ethernet", Gpio::G11, PAL_MODE_ALTERNATE(0xb));
	/* TXD0 */
	efiSetPadMode("Ethernet", Gpio::G13, PAL_MODE_ALTERNATE(0xb));
	/* TXD1 */
	efiSetPadMode("Ethernet", Gpio::B13, PAL_MODE_ALTERNATE(0xb));

	/* RXD0 */
	efiSetPadMode("Ethernet",  Gpio::C4, PAL_MODE_ALTERNATE(0xb));
	/* RXD1 */
	efiSetPadMode("Ethernet",  Gpio::C5, PAL_MODE_ALTERNATE(0xb));
	/* CSR DV */
	efiSetPadMode("Ethernet",  Gpio::A7, PAL_MODE_ALTERNATE(0xb));

	/* MDIO */
	efiSetPadMode("Ethernet",  Gpio::A2, PAL_MODE_ALTERNATE(0xb));
	/* MDC */
	efiSetPadMode("Ethernet",  Gpio::C1, PAL_MODE_ALTERNATE(0xb));

	/* REF_CLK */
	efiSetPadMode("Ethernet",  Gpio::A1, PAL_MODE_ALTERNATE(0xb));
}

void setup_custom_board_overrides() {
	custom_board_preHalInit = nucleo_f429_preHalInit;
}

extern "C" {

void OpenBLT__early_init() {
	nucleo_f429_preHalInit();
}

}
