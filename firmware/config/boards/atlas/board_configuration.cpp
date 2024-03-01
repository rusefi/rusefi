#include "pch.h"

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

void setBoardConfigOverrides() {
	engineConfiguration->is_enabled_spi_4 = true;
	engineConfiguration->spi4sckPin = Gpio::E2;
	engineConfiguration->spi4misoPin = Gpio::E5;
	engineConfiguration->spi4mosiPin = Gpio::E6;
}
