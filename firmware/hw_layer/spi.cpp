/**
 * @file    spi.cpp
 * @brief   Hardware package entry point
 *
 * @date May 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#if HAL_USE_SPI
/* zero index is SPI_NONE */
extern bool isSpiInitialized[SPI_TOTAL_COUNT + 1];

/* these are common adapters for engineConfiguration access, move to some common file? */
brain_pin_e getMisoPin(spi_device_e device) {
	switch(device) {
	case SPI_DEVICE_1:
		return engineConfiguration->spi1misoPin;
	case SPI_DEVICE_2:
		return engineConfiguration->spi2misoPin;
	case SPI_DEVICE_3:
		return engineConfiguration->spi3misoPin;
	case SPI_DEVICE_4:
		return engineConfiguration->spi4misoPin;
	case SPI_DEVICE_5:
		return engineConfiguration->spi5misoPin;
	case SPI_DEVICE_6:
		return engineConfiguration->spi6misoPin;
	default:
		break;
	}
	return Gpio::Unassigned;
}

brain_pin_e getMosiPin(spi_device_e device) {
	switch(device) {
	case SPI_DEVICE_1:
		return engineConfiguration->spi1mosiPin;
	case SPI_DEVICE_2:
		return engineConfiguration->spi2mosiPin;
	case SPI_DEVICE_3:
		return engineConfiguration->spi3mosiPin;
	case SPI_DEVICE_4:
		return engineConfiguration->spi4mosiPin;
	case SPI_DEVICE_5:
		return engineConfiguration->spi5mosiPin;
	case SPI_DEVICE_6:
		return engineConfiguration->spi6mosiPin;
	default:
		break;
	}
	return Gpio::Unassigned;
}

brain_pin_e getSckPin(spi_device_e device) {
	switch(device) {
	case SPI_DEVICE_1:
		return engineConfiguration->spi1sckPin;
	case SPI_DEVICE_2:
		return engineConfiguration->spi2sckPin;
	case SPI_DEVICE_3:
		return engineConfiguration->spi3sckPin;
	case SPI_DEVICE_4:
		return engineConfiguration->spi4sckPin;
	case SPI_DEVICE_5:
		return engineConfiguration->spi5sckPin;
	case SPI_DEVICE_6:
		return engineConfiguration->spi6sckPin;
	default:
		break;
	}
	return Gpio::Unassigned;
}

/**
 * @return NULL if SPI device not specified
 */
SPIDriver * getSpiDevice(spi_device_e spiDevice) {
	if (spiDevice == SPI_NONE) {
		return nullptr;
	}
#if STM32_SPI_USE_SPI1
	if (spiDevice == SPI_DEVICE_1) {
		return &SPID1;
	}
#endif
#if STM32_SPI_USE_SPI2
	if (spiDevice == SPI_DEVICE_2) {
		return &SPID2;
	}
#endif
#if STM32_SPI_USE_SPI3
	if (spiDevice == SPI_DEVICE_3) {
		return &SPID3;
	}
#endif
#if STM32_SPI_USE_SPI4
	if (spiDevice == SPI_DEVICE_4) {
		return &SPID4;
	}
#endif
#if STM32_SPI_USE_SPI5
	if (spiDevice == SPI_DEVICE_5) {
		return &SPID5;
	}
#endif
#if STM32_SPI_USE_SPI6
	if (spiDevice == SPI_DEVICE_6) {
		return &SPID6;
	}
#endif
	firmwareError(ObdCode::CUSTOM_ERR_UNEXPECTED_SPI, "Unexpected SPI device: %d", spiDevice);
	return nullptr;
}

/**
 * Only one consumer can use SPI bus at a given time
 */
void lockSpi(spi_device_e device) {
	efiAssertVoid(ObdCode::CUSTOM_STACK_SPI, hasLotsOfRemainingStack(), "lockSpi");
	spiAcquireBus(getSpiDevice(device));
}

void unlockSpi(spi_device_e device) {
	spiReleaseBus(getSpiDevice(device));
}

void stopSpi(spi_device_e device) {
	if (!isSpiInitialized[device]) {
		return; // not turned on
	}
	isSpiInitialized[device] = false;
	efiSetPadUnused(getSckPin(device));
	efiSetPadUnused(getMisoPin(device));
	efiSetPadUnused(getMosiPin(device));
}

void initSpiModules() {
	if (engineConfiguration->is_enabled_spi_1) {
		 turnOnSpi(SPI_DEVICE_1);
	}
	if (engineConfiguration->is_enabled_spi_2) {
		turnOnSpi(SPI_DEVICE_2);
	}
	if (engineConfiguration->is_enabled_spi_3) {
		turnOnSpi(SPI_DEVICE_3);
	}
	if (engineConfiguration->is_enabled_spi_4) {
		turnOnSpi(SPI_DEVICE_4);
	}
	if (engineConfiguration->is_enabled_spi_5) {
		turnOnSpi(SPI_DEVICE_5);
	}
	if (engineConfiguration->is_enabled_spi_6) {
		turnOnSpi(SPI_DEVICE_6);
	}
}

void stopSpiModules() {
	if (isConfigurationChanged(is_enabled_spi_1)) {
		stopSpi(SPI_DEVICE_1);
	}

	if (isConfigurationChanged(is_enabled_spi_2)) {
		stopSpi(SPI_DEVICE_2);
	}

	if (isConfigurationChanged(is_enabled_spi_3)) {
		stopSpi(SPI_DEVICE_3);
	}

	if (isConfigurationChanged(is_enabled_spi_4)) {
		stopSpi(SPI_DEVICE_4);
	}

	if (isConfigurationChanged(is_enabled_spi_5)) {
		stopSpi(SPI_DEVICE_5);
	}

	if (isConfigurationChanged(is_enabled_spi_6)) {
		stopSpi(SPI_DEVICE_6);
	}
}

void printSpiConfig(const char *msg, spi_device_e device) {
	efiPrintf("%s %s mosi=%s", msg, getSpi_device_e(device), hwPortname(getMosiPin(device)));
	efiPrintf("%s %s miso=%s", msg, getSpi_device_e(device), hwPortname(getMisoPin(device)));
	efiPrintf("%s %s sck=%s",  msg, getSpi_device_e(device), hwPortname(getSckPin(device)));
}

#endif // HAL_USE_SPI
