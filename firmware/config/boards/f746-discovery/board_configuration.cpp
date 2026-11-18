#include "pch.h"
#include "board_overrides.h"

Gpio getWarningLedPin() {
	// this board has no warning led
	return Gpio::Unassigned;
}

Gpio getCommsLedPin() {
	return Gpio::Unassigned;
}

Gpio getRunningLedPin() {
	return Gpio::Unassigned;
}

#if STM32_USB_USE_OTG2 && defined(BOARD_OTG2_USES_ULPI)
// USB3320 ULPI PHY on OTG_HS, pinout matches the stock ChibiOS board.h
static const struct {
	const char *name;
	Gpio pin;
} ulpiPins[] = {
	{ "ULPI CK", Gpio::A5 },
	{ "ULPI D0", Gpio::A3 },
	{ "ULPI D1", Gpio::B0 },
	{ "ULPI D2", Gpio::B1 },
	{ "ULPI D3", Gpio::B10 },
	{ "ULPI D4", Gpio::B11 },
	{ "ULPI D5", Gpio::B12 },
	{ "ULPI D6", Gpio::B13 },
	{ "ULPI D7", Gpio::B5 },
	{ "ULPI STP", Gpio::C0 },
	{ "ULPI DIR", Gpio::C2 },
	{ "ULPI NXT", Gpio::H4 },
};

static void f746_discovery_boardInitHardware() {
	// board.c already muxed these at boot; claim them so they cannot be
	// assigned to anything else in the tune
	for (const auto& p : ulpiPins) {
		efiSetPadMode(p.name, p.pin, PAL_MODE_ALTERNATE(10) | PAL_STM32_OSPEED_HIGHEST);
	}
}
#endif

void setup_custom_board_overrides() {
#if STM32_USB_USE_OTG2 && defined(BOARD_OTG2_USES_ULPI)
	custom_board_InitHardware = f746_discovery_boardInitHardware;
#endif
}
