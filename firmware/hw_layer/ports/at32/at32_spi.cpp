/**
 * @file	stm32_common_spi.cpp
 * @brief	Low level common STM32 code
 *
 * @date Mar 28, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#if HAL_USE_SPI
bool isSpiInitialized[SPI_TOTAL_COUNT] = { false, false, false, false, false, false };

struct af_pairs {
	brain_pin_e pin;
	uint8_t af;
};

#define AT32_SPI_MAX_POSSIBLE_PINS_FOR_FX	9

struct spi_af {
	af_pairs cs[AT32_SPI_MAX_POSSIBLE_PINS_FOR_FX];		/* full or Gpio::Invalid terminated list of possible pins & af for CS function */
	af_pairs sck[AT32_SPI_MAX_POSSIBLE_PINS_FOR_FX];	/* full or Gpio::Invalid terminated list of possible pins & af for SCK function */
	af_pairs miso[AT32_SPI_MAX_POSSIBLE_PINS_FOR_FX];	/* full or Gpio::Invalid terminated list of possible pins & af for MISO function */
	af_pairs mosi[AT32_SPI_MAX_POSSIBLE_PINS_FOR_FX];	/* full or Gpio::Invalid terminated list of possible pins & af for MOSI function */
};

#if STM32_SPI_USE_SPI1
static const struct spi_af spi1_af = {
	.cs		= {{Gpio::A4,  5},	{Gpio::A15, 5},	{Gpio::E12, 4},	{Gpio::Invalid, 0}},
	.sck	= {{Gpio::A5,  5},	{Gpio::B3,  5},	{Gpio::E13, 4},	{Gpio::Invalid, 0}},
	.miso	= {{Gpio::A6,  5},	{Gpio::B4,  5},	{Gpio::E14, 4},	{Gpio::G0,  5},	{Gpio::Invalid, 0}},
	.mosi	= {{Gpio::A7,  5},	{Gpio::B5,  5},	{Gpio::E15, 4},	{Gpio::G1,  5},	{Gpio::Invalid, 0}}
};
#endif

#if STM32_SPI_USE_SPI2
static const struct spi_af spi2_af = {
	.cs		= {{Gpio::A11, 5},	{Gpio::B9,  5},					{Gpio::B12, 5}, 				{Gpio::D1,  7}, {Gpio::D2,  7},	{Gpio::Invalid, 0}},
	.sck	= {{Gpio::A9,  5},	{Gpio::B1,  6},	{Gpio::B10, 5},	{Gpio::B13, 5},	{Gpio::C7,  5},	{Gpio::D1,  6},	{Gpio::D3,  5},	{Gpio::Invalid, 0}},
	.miso	= {{Gpio::A12, 5},									{Gpio::B14, 5},					{Gpio::C2,  5},	{Gpio::D3,  6},	{Gpio::Invalid, 0}},
	.mosi	= {{Gpio::A10, 5},									{Gpio::B15, 5},	{Gpio::C1,  7},	{Gpio::C3,  5},	{Gpio::D4,  6},	{Gpio::Invalid, 0}}
};
#endif

#if STM32_SPI_USE_SPI3
static const struct spi_af spi3_af = {
	.cs		= {{Gpio::A4,  6},	{Gpio::A15, 6},	{Gpio::Invalid, 0}},
	.sck	= {{Gpio::B3,  6},	{Gpio::B12, 7},	{Gpio::C10, 6},	{Gpio::Invalid, 0}},
	.miso	= {{Gpio::A13, 6},	{Gpio::B4,  6},	{Gpio::C11, 6},	{Gpio::Invalid, 0}},
	.mosi	= {{Gpio::A14, 6},	{Gpio::B0,  7},	{Gpio::B2,  7},	{Gpio::B5,  6},	{Gpio::C1,  5},	{Gpio::C12, 6},	{Gpio::D0,  6},	{Gpio::D6,  5},	{Gpio::Invalid, 0}}
};
#endif

#if STM32_SPI_USE_SPI4
static const struct spi_af spi4_af = {
	.cs		= {{Gpio::B6,  6},	{Gpio::B12, 6},	{Gpio::E4,  5},	{Gpio::E11, 5},	{Gpio::G14, 6},	{Gpio::Invalid, 0}},
	.sck	= {{Gpio::B7,  6},	{Gpio::B13, 6},	{Gpio::E2,  5},	{Gpio::E12, 5},	{Gpio::G11, 6},	{Gpio::Invalid, 0}},
	.miso	= {{Gpio::A11, 6},	{Gpio::B8,  6},	{Gpio::D0,  5},	{Gpio::E5,  5},	{Gpio::E13, 5},	{Gpio::G12, 6},	{Gpio::Invalid, 0}},
	.mosi	= {{Gpio::A1,  5},	{Gpio::B9,  6},	{Gpio::E6,  5},	{Gpio::E14, 5},	{Gpio::G13, 6},	{Gpio::Invalid, 0}}
};
#endif

static int findAfForPin(const struct af_pairs *list, brain_pin_e pin)
{
	int i;

	if (list == NULL) {
		return -1;
	}

	/* scan all list or until Gpio::Invalid */
	for (i = 0; (i < AT32_SPI_MAX_POSSIBLE_PINS_FOR_FX) && (list[i].pin != Gpio::Invalid); i++) {
		if (list[i].pin == pin) {
			return list[i].af;
		}
	}
	return -1;
}

static const struct spi_af *getAfListForSpi(SPIDriver *driver)
{
#if STM32_SPI_USE_SPI1
	if (driver == &SPID1) {
		return &spi1_af;
	}
#endif
#if STM32_SPI_USE_SPI2
	if (driver == &SPID2) {
		return &spi2_af;
	}
#endif
#if STM32_SPI_USE_SPI3
	if (driver == &SPID3) {
		return &spi3_af;
	}
#endif
#if STM32_SPI_USE_SPI4
	if (driver == &SPID4) {
		return &spi4_af;
	}
#endif
	criticalError("SPI interface is not available");
	return NULL;
}

#if 0
/* Currenty unused */
static int getSpiCsAf(SPIDriver *driver, brain_pin_e pin)
{
	const struct spi_af *af = getAfListForSpi(driver);

	if (af == NULL)
		return -1;

	return findAfForPin(af->cs, pin);
}
#endif

static int getSpiSckAf(SPIDriver *driver, brain_pin_e pin)
{
	const struct spi_af *af = getAfListForSpi(driver);

	if (af == NULL)
		return -1;

	return findAfForPin(af->sck, pin);
}

static int getSpiMisoAf(SPIDriver *driver, brain_pin_e pin)
{
	const struct spi_af *af = getAfListForSpi(driver);

	if (af == NULL)
		return -1;

	return findAfForPin(af->miso, pin);
}

static int getSpiMosiAf(SPIDriver *driver, brain_pin_e pin)
{
	const struct spi_af *af = getAfListForSpi(driver);

	if (af == NULL)
		return -1;

	return findAfForPin(af->mosi, pin);
}

/* these are common adapters for engineConfiguration access, move to some common file? */
brain_pin_e getMisoPin(spi_device_e device) {
	switch(device) {
	case SPI_DEVICE_1:
		return engineConfiguration->spi1misoPin;
	case SPI_DEVICE_2:
		return engineConfiguration->spi2misoPin;
	case SPI_DEVICE_3:
		return engineConfiguration->spi3misoPin;
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
	default:
		break;
	}
	return Gpio::Unassigned;
}

void turnOnSpi(spi_device_e device) {
	if (isSpiInitialized[device])
		return; // already initialized
	isSpiInitialized[device] = true;
	if (device == SPI_DEVICE_1) {
// todo: introduce a nice structure with all fields for same SPI
#if STM32_SPI_USE_SPI1
//		scheduleMsg(&logging, "Turning on SPI1 pins");
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
		//scheduleMsg(&logging, "Turning on SPI2 pins");
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
		//scheduleMsg(&logging, "Turning on SPI3 pins");
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
		//scheduleMsg(&logging, "Turning on SPI4 pins");
		/* there are no configuration fields for SPI4 in engineConfiguration, rely on board init code
		 * it should set proper functions for SPI4 pins */
#else
		criticalError("SPI4 not available in this binary");
#endif /* STM32_SPI_USE_SPI4 */
	}
}

void initSpiModule(SPIDriver *driver, brain_pin_e sck, brain_pin_e miso,
		brain_pin_e mosi,
		int sckMode,
		int mosiMode,
		int misoMode) {

	int sckAf = getSpiSckAf(driver, sck);
	int mosiAf = getSpiMosiAf(driver, mosi);
	int misoAf = getSpiMisoAf(driver, miso);

	if ((sckAf < 0) || (mosiAf < 0) || (misoAf < 0)) {
		criticalError("Incorrect SPI pin configuration");
	}

	efiSetPadMode("SPI clock", sck,
		PAL_MODE_ALTERNATE(sckAf) | sckMode | PAL_STM32_OSPEED_HIGHEST);

	efiSetPadMode("SPI master out", mosi,
		PAL_MODE_ALTERNATE(mosiAf) | mosiMode | PAL_STM32_OSPEED_HIGHEST);

	// Activate the internal pullup on MISO: SD cards indicate "busy" by holding MOSI low,
	// so in case there is no SD card installed, the line could float low and indicate that
	// the (non existent) card is busy.  We pull the line high to indicate "not busy" in case
	// of a missing card.
	efiSetPadMode("SPI master in ", miso,
		PAL_MODE_ALTERNATE(misoAf) | misoMode | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_PUPDR_PULLUP);
}

void initSpiCsNoOccupy(SPIConfig *spiConfig, brain_pin_e csPin) {
	ioportid_t port = getHwPort("spi", csPin);
	ioportmask_t pin = getHwPin("spi", csPin);
	spiConfig->ssport = port;
	spiConfig->sspad = pin;
}

void initSpiCs(SPIConfig *spiConfig, brain_pin_e csPin) {
	/* TODO: why this is here? */
	spiConfig->end_cb = nullptr;

	initSpiCsNoOccupy(spiConfig, csPin);
	efiSetPadMode("chip select", csPin, PAL_STM32_MODE_OUTPUT);
}

// SD cards are good up to 25MHz in "slow" mode, and 50MHz in "fast" mode
// 168mhz F4:
// Slow mode is 10.5 or 5.25 MHz, depending on which SPI device
// Fast mode is 42 or 21 MHz
// 216mhz F7:
// Slow mode is 13.5 or 6.75 MHz
// Fast mode is 54 or 27 MHz (technically out of spec, needs testing!)
SPIConfig mmc_hs_spicfg = {
		.circular = false,
		.end_cb = NULL,
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

#endif /* HAL_USE_SPI */
