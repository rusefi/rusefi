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

#if defined(HARDWARE_CI) && defined(HW_NUCLEO_F767)
static void nucleoHardwareCiConfigOverrides() {
	// MINIMAL_PINS has no MAP input, so the fast ADC would never start.
	// PC3 supports ADC2 (fast) and ADC1 (slow), and avoids Ethernet pins.
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_13;
}
#endif

void setup_custom_board_overrides() {
	custom_board_preHalInit = nucleo_f429_preHalInit;
#if defined(HARDWARE_CI) && defined(HW_NUCLEO_F767)
	custom_board_ConfigOverrides = nucleoHardwareCiConfigOverrides;
#endif
}

extern "C" {

void OpenBLT__early_init() {
	nucleo_f429_preHalInit();
}

}
