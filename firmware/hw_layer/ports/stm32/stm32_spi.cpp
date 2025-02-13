/**
 * @file	stm32_common_spi.cpp
 * @brief	Low level common STM32 code
 *
 * @date Mar 28, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#if HAL_USE_SPI
/* zero index is SPI_NONE */
bool isSpiInitialized[SPI_TOTAL_COUNT + 1] = { true, false, false, false, false, false, false };

static int getSpiAf(SPIDriver *driver) {
#if STM32_SPI_USE_SPI1
	if (driver == &SPID1) {
		return EFI_SPI1_AF;
	}
#endif
#if STM32_SPI_USE_SPI2
	if (driver == &SPID2) {
		return EFI_SPI2_AF;
	}
#endif
#if STM32_SPI_USE_SPI3
	if (driver == &SPID3) {
		return EFI_SPI3_AF;
	}
#endif
#if STM32_SPI_USE_SPI4
	if (driver == &SPID4) {
		return EFI_SPI4_AF;
	}
#endif
#if STM32_SPI_USE_SPI5
	if (driver == &SPID5) {
		return EFI_SPI5_AF;
	}
#endif
#if STM32_SPI_USE_SPI6
	if (driver == &SPID6) {
		return EFI_SPI6_AF;
	}
#endif
	criticalError("SPI AF not available");
	return -1;
}

void turnOnSpi(spi_device_e device) {
	if (isSpiInitialized[device])
		return; // already initialized
	isSpiInitialized[device] = true;
	if (device == SPI_DEVICE_1) {
// todo: introduce a nice structure with all fields for same SPI
#if STM32_SPI_USE_SPI1
//	scheduleMsg(&logging, "Turning on SPI1 pins");
		initSpiModule(&SPID1, getSckPin(device),
				getMisoPin(device),
				getMosiPin(device),
				engineConfiguration->spi1SckMode,
				engineConfiguration->spi1MosiMode,
				engineConfiguration->spi1MisoMode);
#else
		criticalError("SPI1 not available in this binary");
#endif /* STM32_SPI_USE_SPI1 */
	}
	if (device == SPI_DEVICE_2) {
#if STM32_SPI_USE_SPI2
//	scheduleMsg(&logging, "Turning on SPI2 pins");
		initSpiModule(&SPID2, getSckPin(device),
				getMisoPin(device),
				getMosiPin(device),
				engineConfiguration->spi2SckMode,
				engineConfiguration->spi2MosiMode,
				engineConfiguration->spi2MisoMode);
#else
		criticalError("SPI2 not available in this binary");
#endif /* STM32_SPI_USE_SPI2 */
	}
	if (device == SPI_DEVICE_3) {
#if STM32_SPI_USE_SPI3
//	scheduleMsg(&logging, "Turning on SPI3 pins");
		initSpiModule(&SPID3, getSckPin(device),
				getMisoPin(device),
				getMosiPin(device),
				engineConfiguration->spi3SckMode,
				engineConfiguration->spi3MosiMode,
				engineConfiguration->spi3MisoMode);
#else
		criticalError("SPI3 not available in this binary");
#endif /* STM32_SPI_USE_SPI3 */
	}
	if (device == SPI_DEVICE_4) {
#if STM32_SPI_USE_SPI4
		initSpiModule(&SPID4, getSckPin(device),
				getMisoPin(device),
				getMosiPin(device),
				engineConfiguration->spi4SckMode,
				engineConfiguration->spi4MosiMode,
				engineConfiguration->spi4MisoMode);
#else
		criticalError("SPI4 not available in this binary");
#endif /* STM32_SPI_USE_SPI4 */
	}
	if (device == SPI_DEVICE_5) {
#if STM32_SPI_USE_SPI5
		initSpiModule(&SPID5, getSckPin(device),
				getMisoPin(device),
				getMosiPin(device),
				engineConfiguration->spi5SckMode,
				engineConfiguration->spi5MosiMode,
				engineConfiguration->spi5MisoMode);
#else
		criticalError("SPI5 not available in this binary");
#endif /* STM32_SPI_USE_SPI5 */
	}
	if (device == SPI_DEVICE_6) {
#if STM32_SPI_USE_SPI6
		initSpiModule(&SPID6, getSckPin(device),
				getMisoPin(device),
				getMosiPin(device),
				engineConfiguration->spi6SckMode,
				engineConfiguration->spi6MosiMode,
				engineConfiguration->spi6MisoMode);
#else
		criticalError("SPI5 not available in this binary");
#endif /* STM32_SPI_USE_SPI5 */
	}
}

void initSpiModule(SPIDriver *driver,
		brain_pin_e sck, brain_pin_e miso, brain_pin_e mosi,
		int sckMode, int mosiMode, int misoMode) {

	/**
	 * See https://github.com/rusefi/rusefi/pull/664/
	 *
	 * Info on the silicon defect can be found in this document, section 2.5.2:
	 * https://www.st.com/content/ccc/resource/technical/document/errata_sheet/0a/98/58/84/86/b6/47/a2/DM00037591.pdf/files/DM00037591.pdf/jcr:content/translations/en.DM00037591.pdf
	 */
	efiSetPadMode("SPI CLK ", sck,	PAL_MODE_ALTERNATE(getSpiAf(driver)) | sckMode | PAL_STM32_OSPEED_HIGHEST);

	efiSetPadMode("SPI MOSI", mosi, PAL_MODE_ALTERNATE(getSpiAf(driver)) | mosiMode | PAL_STM32_OSPEED_HIGHEST);

	// Activate the internal pullup on MISO: SD cards indicate "busy" by holding MOSI low,
	// so in case there is no SD card installed, the line could float low and indicate that
	// the (non existent) card is busy.  We pull the line high to indicate "not busy" in case
	// of a missing card.
	efiSetPadMode("SPI MISO", miso, PAL_MODE_ALTERNATE(getSpiAf(driver)) | misoMode | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_PUPDR_PULLUP);
}

void initSpiCsNoOccupy(SPIConfig *spiConfig, brain_pin_e csPin) {
	ioportid_t port = getHwPort("spi", csPin);
	ioportmask_t pin = getHwPin("spi", csPin);
	spiConfig->ssport = port;
	spiConfig->sspad = pin;
}

void initSpiCs(SPIConfig *spiConfig, brain_pin_e csPin) {
	/* TODO: why this is here? */
#ifdef _CHIBIOS_RT_CONF_VER_6_1_
	spiConfig->end_cb = nullptr;
#else
	spiConfig->data_cb = nullptr;
	spiConfig->error_cb = nullptr;
#endif

	initSpiCsNoOccupy(spiConfig, csPin);
	efiSetPadMode("chip select", csPin, PAL_STM32_MODE_OUTPUT);
}

int spiGetBaseClock(SPIDriver *spip)
{
#if STM32_SPI_USE_SPI1
	if (spip == &SPID1) {
		// APB2
		return STM32_PCLK2;
	}
#endif
#if STM32_SPI_USE_SPI2
	if (spip == &SPID2) {
		// APB1
		return STM32_PCLK1;
	}
#endif
#if STM32_SPI_USE_SPI3
	if (spip == &SPID3) {
		// APB1
		return STM32_PCLK1;
	}
#endif
#if STM32_SPI_USE_SPI4
	if (spip == &SPID4) {
		// APB2
		return STM32_PCLK2;
	}
#endif
#if STM32_SPI_USE_SPI5
	if (spip == &SPID5) {
		// APB2
		return STM32_PCLK2;
	}
#endif
#if STM32_SPI_USE_SPI6
	if (spip == &SPID6) {
		// APB2
		return STM32_PCLK2;
	}
#endif

	return 0;
}

#ifdef STM32H7XX

int spiCalcClockDiv(SPIDriver*, SPIConfig*, unsigned int)
{
	// TODO: implement
	return -1;
}

#else

int spiCalcClockDiv(SPIDriver *spip, SPIConfig *spiConfig, unsigned int clk)
{
	if (clk == 0) {
		return -1;
	}

	unsigned int baseClock = spiGetBaseClock(spip);

	if (baseClock == 0) {
		return -1;
	}

	// round down
	int div = (baseClock + clk - 1) / clk;

	spiConfig->cr1 &= ~SPI_CR1_BR_Msk;
	if (div <= 2) {
		spiConfig->cr1 |= SPI_BaudRatePrescaler_2;
	} else if (div <= 4) {
		spiConfig->cr1 |= SPI_BaudRatePrescaler_4;
	} else if (div <= 8) {
		spiConfig->cr1 |= SPI_BaudRatePrescaler_8;
	} else if (div <= 16) {
		spiConfig->cr1 |= SPI_BaudRatePrescaler_16;
	} else if (div <= 32) {
		spiConfig->cr1 |= SPI_BaudRatePrescaler_32;
	} else if (div <= 64) {
		spiConfig->cr1 |= SPI_BaudRatePrescaler_64;
	} else if (div <= 128) {
		spiConfig->cr1 |= SPI_BaudRatePrescaler_128;
	} else {
		spiConfig->cr1 |= SPI_BaudRatePrescaler_256;
	}

	return 0;
}

#endif

#ifdef STM32H7XX
// H7 SPI clock is set to 80MHz
// fast mode is 80mhz/2 = 40MHz
SPIConfig mmc_hs_spicfg = {
		.circular = false,
#ifdef _CHIBIOS_RT_CONF_VER_6_1_
		.end_cb = NULL,
#else
        .slave = false,
        .data_cb = NULL,
        .error_cb = NULL,
#endif
		.ssport = NULL,
		.sspad = 0,
		.cfg1 = 7 // 8 bits per byte
			| 0 /* MBR = 0, divider = 2 */,
		.cfg2 = 0
};

// Slow mode is 80mhz/4 = 20MHz
SPIConfig mmc_ls_spicfg = {
		.circular = false,
		.end_cb = NULL,
		.ssport = NULL,
		.sspad = 0,
		.cfg1 = 7 // 8 bits per byte
			| SPI_CFG1_MBR_0 /* MBR = 001, divider = 4 */,
		.cfg2 = 0
};

#else /* not STM32H7XX */

// SD cards are good up to 25MHz in "slow" mode, and 50MHz in "fast" mode
// 168mhz F4:
// Slow mode is 10.5 or 5.25 MHz, depending on which SPI device
// Fast mode is 42 or 21 MHz
// 216mhz F7:
// Slow mode is 13.5 or 6.75 MHz
// Fast mode is 54 or 27 MHz (technically out of spec, needs testing!)
SPIConfig mmc_hs_spicfg = {
		.circular = false,
#ifdef _CHIBIOS_RT_CONF_VER_6_1_
	.end_cb = NULL,
#else
        .slave = false,
        .data_cb = NULL,
        .error_cb = NULL,
#endif
		.ssport = NULL,
		.sspad = 0,
		.cr1 = SPI_BaudRatePrescaler_2,
		.cr2 = 0
};

SPIConfig mmc_ls_spicfg = {
		.circular = false,
		.end_cb = NULL,
		.ssport = NULL,
		.sspad = 0,
		.cr1 = SPI_BaudRatePrescaler_8,
		.cr2 = 0
};
#endif

#endif /* HAL_USE_SPI */
