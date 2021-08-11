/**
 * @file	poten.cpp
 * @brief	MCP42010 digital potentiometer driver
 *
 * @date Mar 16, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "os_access.h"
#include "poten.h"
#include "eficonsole.h"
#include "hardware.h"
#include "mpu_util.h"

#if HAL_USE_SPI

/**
 * MCP42010 digital potentiometer driver
 *
 *
 * 1	CS		pin select						PB12		PA10
 * 2	SCK		serial clock					PA5			PC10
 * 3	SI		serial input		(MOSI)		PA7			PC12
 * 4	Vss 	ground
 * 5	PB1
 * 6	PW1
 * 7	PA1
 * 8	PA0
 * 9	PW0
 * 10	PB0
 * 11	RS		Reset
 *
 * 14	Vdd 	V input
 *
 * Rwa = 10000 * (256 - d) / 256 + 52
 * d = 256 - (Rwa - 52) * 256 / 10000
 *
 */

/* Low speed SPI configuration (281.250kHz, CPHA=0, CPOL=0, MSb first).*/

#if defined(STM32F7XX)
#define SPI_POT_CONFIG SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_CRCL
#else /* defined(STM32F4XX) */
#define SPI_POT_CONFIG SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_DFF
#endif /* defined(STM32F4XX) */

#if EFI_POTENTIOMETER
static Mcp42010Driver potConfig[DIGIPOT_COUNT];

void initPotentiometer(Mcp42010Driver *driver, SPIDriver *spi, brain_pin_e csPin) {
	driver->spiConfig.cr1 = SPI_POT_CONFIG;
	driver->spi = spi;
	initSpiCs(&driver->spiConfig, csPin);
}

static int getPotStep(int resistanceWA) {
	return 256 - (int) ((resistanceWA - 52) * 256 / 10000);
}

static void sendToPot(Mcp42010Driver *driver, int channel, int value) {
	lockSpi(SPI_NONE);
	spiStart(driver->spi, &driver->spiConfig);
	spiSelect(driver->spi);
	int word = (17 + channel) * 256 + value;
	spiSend(driver->spi, 1, &word);
	spiUnselect(driver->spi);
	spiStop(driver->spi);
	unlockSpi(SPI_NONE);
}

void setPotResistance(Mcp42010Driver *driver, int channel, int resistance) {
	int value = getPotStep(resistance);

	efiPrintf("Sending to potentiometer%d: %d for R=%d", channel, value, resistance);
	sendToPot(driver, channel, value);
}

static void setPotResistanceCommand(int index, int value) {
	setPotResistance(&potConfig[index / 2], index % 2, value);
}

static void setPotValue1(int value) {
	sendToPot(&potConfig[0], 1, value);
}

#endif /* EFI_POTENTIOMETER */

void initPotentiometers(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if EFI_POTENTIOMETER
	if (CONFIG(digitalPotentiometerSpiDevice) == SPI_NONE) {
		efiPrintf("digiPot spi disabled");
		return;
	}
	turnOnSpi(CONFIG(digitalPotentiometerSpiDevice));

	for (int i = 0; i < DIGIPOT_COUNT; i++) {
		brain_pin_e csPin = CONFIG(digitalPotentiometerChipSelect)[i];
		if (!isBrainPinValid(csPin)) {
			continue;
                }

		SPIDriver *driver = getSpiDevice(CONFIG(digitalPotentiometerSpiDevice));
		if (driver == NULL) {
			// error already reported
			return;
		}

		initPotentiometer(&potConfig[i], driver, csPin);
	}

	addConsoleActionII("pot", setPotResistanceCommand);

	addConsoleActionI("potd1", setPotValue1);

	setPotResistance(&potConfig[0], 0, 3000);
	setPotResistance(&potConfig[0], 1, 7000);
#endif
}

#endif /* HAL_USE_SPI */
