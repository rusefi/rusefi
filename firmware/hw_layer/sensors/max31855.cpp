/**
 * @file max31855.cpp
 * @brief MAX31855 Thermocouple-to-Digital Converter driver
 *
 *
 * http://datasheets.maximintegrated.com/en/ds/MAX31855.pdf
 *
 *
 * Read-only communication over 5MHz SPI
 *
 * @date Sep 17, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "max31855.h"

#include "hardware.h"

#if EFI_PROD_CODE
#include "mpu_util.h"
#endif /* EFI_PROD_CODE */

#if EFI_MAX_31855

#define EGT_ERROR_VALUE -1000

static SPIDriver *driver;

static SPIConfig spiConfig[EGT_CHANNEL_COUNT];

static void showEgtInfo() {
#if EFI_PROD_CODE
	printSpiState(engineConfiguration);

	efiPrintf("EGT spi: %d", CONFIG(max31855spiDevice));

	for (int i = 0; i < EGT_CHANNEL_COUNT; i++) {
		if (isBrainPinValid(CONFIG(max31855_cs)[i])) {
			efiPrintf("%d ETG @ %s", i, hwPortname(CONFIG(max31855_cs)[i]));
		}
	}
#endif
}

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

static max_32855_code getResultCode(uint32_t egtPacket) {
	if ((egtPacket & MC_RESERVED_BITS) != 0) {
		return MC_INVALID;
	} else if ((egtPacket & MC_OPEN_BIT) != 0) {
		return MC_OPEN;
	} else if ((egtPacket & MC_GND_BIT) != 0) {
		return MC_SHORT_GND;
	} else if ((egtPacket & MC_VCC_BIT) != 0) {
		return MC_SHORT_VCC;
	} else {
		return MC_OK;
	}
}

static uint32_t readEgtPacket(int egtChannel) {
	uint32_t egtPacket;
	if (driver == NULL) {
		return 0xFFFFFFFF;
	}

	spiStart(driver, &spiConfig[egtChannel]);
	spiSelect(driver);

	spiReceive(driver, sizeof(egtPacket), &egtPacket);

	spiUnselect(driver);
	spiStop(driver);
	egtPacket = SWAP_UINT32(egtPacket);
	return egtPacket;
}

#define GET_TEMPERATURE_C(x) (((x) >> 18) / 4)

uint16_t getEgtValue(int egtChannel) {
	uint32_t packet = readEgtPacket(egtChannel);
	max_32855_code code = getResultCode(packet);
	if (code != MC_OK) {
		return EGT_ERROR_VALUE + code;
	} else {
		return GET_TEMPERATURE_C(packet);
	}
}

static void egtRead() {

	if (driver == NULL) {
		efiPrintf("No SPI selected for EGT");
		return;
	}

	efiPrintf("Reading egt");

	uint32_t egtPacket = readEgtPacket(0);

	max_32855_code code = getResultCode(egtPacket);

	efiPrintf("egt %x code=%d %s", egtPacket, code, getMcCode(code));

	if (code != MC_INVALID) {
		int refBits = ((egtPacket & 0xFFFF) / 16); // bits 15:4
		float refTemp = refBits / 16.0;
		efiPrintf("reference temperature %.2f", refTemp);

		efiPrintf("EGT temperature %d", GET_TEMPERATURE_C(egtPacket));
	}
}

void initMax31855(spi_device_e device, egt_cs_array_t max31855_cs) {
	driver = getSpiDevice(device);
	if (driver == NULL) {
		// error already reported
		return;
	}

	// todo:spi device is now enabled separately - should probably be enabled here

	addConsoleAction("egtinfo", (Void) showEgtInfo);

	addConsoleAction("egtread", (Void) egtRead);

	for (int i = 0; i < EGT_CHANNEL_COUNT; i++) {
		if (isBrainPinValid(max31855_cs[i])) {

			initSpiCs(&spiConfig[i], max31855_cs[i]);

			spiConfig[i].cr1 = getSpiPrescaler(_5MHz, device);
		}
	}
}

#endif /* EFI_MAX_31855 */
