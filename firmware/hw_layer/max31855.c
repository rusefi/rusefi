/**
 * @file max31855.c
 * @brief MAX31855 Thermocouple-to-Digital Converter driver
 *
 *
 * http://datasheets.maximintegrated.com/en/ds/MAX31855.pdf
 *
 *
 * Read-only communication over 5MHz SPI
 *
 * @date Sep 17, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "max31855.h"
#include "pin_repository.h"
#include "settings.h"
#include "hardware.h"
// that's for swap
#include "tunerstudio_algo.h"

#if EFI_MAX_31855

static Logging logger;

static SPIConfig spiConfig[MAX31855_CS_COUNT];

static void showEgtInfo(board_configuration_s *boardConfiguration) {
	printSpiState(&logger, boardConfiguration);

	for (int i = 0; i < MAX31855_CS_COUNT; i++) {
		if (boardConfiguration->max31855_cs[i] != GPIO_NONE) {
			scheduleMsg(&logger, "%d ETG @ %s", i, hwPortname(boardConfiguration->max31855_cs[i]));

		}

	}
}

static uint32_t rx_buff;

// bits D17 and D3 are always expected to be zero
#define MC_RESERVED_BITS 0x20008
#define MC_OPEN_BIT 1
#define MC_GND_BIT 2
#define MC_VCC_BIT 4

typedef enum {
	MC_OK = 0, MC_INVALID = 1, MC_OPEN = 2, MC_SHORT_GND = 3, MC_SHORT_VCC = 4,
} max_32855_code;

static const char * getMcCode(max_32855_code code) {
	switch (code) {
	case MC_OK:
		return "Ok";
	case MC_OPEN:
		return "Open";
	case MC_SHORT_GND:
		return "short gnd";
	case MC_SHORT_VCC:
		return "short VCC";
	default:
		return "invalid";
	}
}

static void egtRead(board_configuration_s *boardConfiguration) {

	scheduleMsg(&logger, "Reading egt");

	SPIDriver *driver = getSpiDevice(SPI_DEVICE_3);

	spiStart(driver, &spiConfig[0]);
	spiSelect(driver);

	spiReceive(driver, 4, &rx_buff);

	spiUnselect(driver);
	spiStop(driver);

	rx_buff = SWAP_UINT32(rx_buff);

	max_32855_code code;

	if ((rx_buff & MC_RESERVED_BITS) != 0) {
		code = MC_INVALID;
	} else if ((rx_buff & MC_OPEN_BIT) != 0) {
		code = MC_OPEN;
	} else if ((rx_buff & MC_GND_BIT) != 0) {
		code = MC_SHORT_GND;
	} else if ((rx_buff & MC_VCC_BIT) != 0) {
		code = MC_SHORT_VCC;
	} else {
		code = MC_OK;
	}

	scheduleMsg(&logger, "egt %x code=%d %s", rx_buff, code, getMcCode(code));

	if (code != MC_INVALID) {
		int refBits = ((rx_buff & 0xFFFF) / 16); // bits 15:4
		float refTemp = refBits / 16.0;
		scheduleMsg(&logger, "reference temperature %f", refTemp);
	}

}

void initMax31855(board_configuration_s *boardConfiguration) {
	initLogging(&logger, "EGT");

	addConsoleActionP("egtinfo", (VoidPtr) showEgtInfo, boardConfiguration);

	addConsoleActionP("egtread", (VoidPtr) egtRead, boardConfiguration);

	turnOnSpi(SPI_DEVICE_3);

	for (int i = 0; i < MAX31855_CS_COUNT; i++) {
		if (boardConfiguration->max31855_cs[i] != GPIO_NONE) {

			initSpiCs(&spiConfig[i], boardConfiguration->max31855_cs[i]);

			spiConfig[i].cr1 = SPI_BaudRatePrescaler_8;
		}
	}
}

#endif /* EFI_MAX_31855 */
