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

struct FixedPin {
	const char *name;
	Gpio pin;
	iomode_t mode;
};

// Pins with fixed on-board functions. board.c already muxed them at boot (stock
// ChibiOS board.h); claiming them keeps them from being assigned in the tune.
#define ULPI_MODE (PAL_MODE_ALTERNATE(10) | PAL_STM32_OSPEED_HIGHEST)
// The board has external pull-ups on the SD lines
#define SDMMC_MODE (PAL_MODE_ALTERNATE(12) | PAL_STM32_OSPEED_HIGHEST)

static const FixedPin fixedPins[] = {
#if STM32_USB_USE_OTG2 && defined(BOARD_OTG2_USES_ULPI)
	// USB3320 ULPI PHY on OTG_HS
	{ "ULPI CK", Gpio::A5, ULPI_MODE },
	{ "ULPI D0", Gpio::A3, ULPI_MODE },
	{ "ULPI D1", Gpio::B0, ULPI_MODE },
	{ "ULPI D2", Gpio::B1, ULPI_MODE },
	{ "ULPI D3", Gpio::B10, ULPI_MODE },
	{ "ULPI D4", Gpio::B11, ULPI_MODE },
	{ "ULPI D5", Gpio::B12, ULPI_MODE },
	{ "ULPI D6", Gpio::B13, ULPI_MODE },
	{ "ULPI D7", Gpio::B5, ULPI_MODE },
	{ "ULPI STP", Gpio::C0, ULPI_MODE },
	{ "ULPI DIR", Gpio::C2, ULPI_MODE },
	{ "ULPI NXT", Gpio::H4, ULPI_MODE },
#endif
#if HAL_USE_SDC && STM32_SDC_USE_SDMMC1
	// microSD slot on SDMMC1
	{ "SD D0", Gpio::C8, SDMMC_MODE },
	{ "SD D1", Gpio::C9, SDMMC_MODE },
	{ "SD D2", Gpio::C10, SDMMC_MODE },
	{ "SD D3", Gpio::C11, SDMMC_MODE },
	{ "SD CK", Gpio::C12, SDMMC_MODE },
	{ "SD CMD", Gpio::D2, SDMMC_MODE },
	// card detect switch, low when a card is inserted; not read by the firmware
	{ "SD detect", Gpio::C13, PAL_MODE_INPUT_PULLUP },
#endif
};

static void f746_discovery_boardInitHardware() {
	for (const auto& p : fixedPins) {
		efiSetPadMode(p.name, p.pin, p.mode);
	}
}

void setup_custom_board_overrides() {
	custom_board_InitHardware = f746_discovery_boardInitHardware;
}
