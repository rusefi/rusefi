#include "pch.h"

Gpio getCommsLedPin() {
	// this board has no comms led
	return Gpio::Unassigned;
}

Gpio getWarningLedPin() {
	// this board has no warning led
	return Gpio::Unassigned;
}

Gpio getRunningLedPin() {
	// this board has no running led
	return Gpio::Unassigned;
}
spi_device_e getWifiSpiDevice() {
	return SPI_DEVICE_4;
}

Gpio getWifiCsPin() {
	return Gpio::E4;
}

Gpio getWifiResetPin() {
	return Gpio::E1;
}

Gpio getWifiIsrPin() {
	return Gpio::E3;
}

static void AtlasBoardConfigOverrides() {
	engineConfiguration->is_enabled_spi_4 = true;
	engineConfiguration->spi4sckPin = Gpio::E2;
	engineConfiguration->spi4misoPin = Gpio::E5;
	engineConfiguration->spi4mosiPin = Gpio::E6;
}

void setup_custom_board_overrides() {
	custom_board_ConfigOverrides = AtlasBoardConfigOverrides;
}
