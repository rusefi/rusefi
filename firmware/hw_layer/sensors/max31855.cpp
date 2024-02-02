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

#include "thread_controller.h"

#define EGT_ERROR_VALUE -1000

static SPIDriver *driver;
static egt_cs_array_t m_cs;

/* TODO: validate */
static SPIConfig spiConfig = {
	.circular = false,
	.end_cb = NULL,
	.ssport = NULL,
	.sspad = 0,
	.cr1 =
		SPI_CR1_8BIT_MODE |
		SPI_CR1_SSM |
		SPI_CR1_SSI |
		((3 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |	/* div = 16 */
		SPI_CR1_MSTR |
		/* SPI_CR1_CPOL | */ // = 0
		SPI_CR1_CPHA | // = 1
		0,
	.cr2 = SPI_CR2_8BIT_MODE
};

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
	if (((egtPacket & MC_RESERVED_BITS) != 0) || (egtPacket == 0x0)) {
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
	brain_pin_e cs = m_cs[egtChannel];

	if ((!isBrainPinValid(cs)) || (driver == NULL)) {
		return 0xFFFFFFFF;
	}

	/* Set proper CS gpio */
	initSpiCsNoOccupy(&spiConfig, cs);

	spiStart(driver, &spiConfig);
	spiSelect(driver);

	spiReceive(driver, sizeof(egtPacket), &egtPacket);

	spiUnselect(driver);
	spiStop(driver);

	egtPacket = SWAP_UINT32(egtPacket);
	return egtPacket;
}

#define GET_TEMPERATURE_C(x) (((x) >> 18) / 4)

static uint16_t getMax31855EgtValue(int egtChannel) {
	uint32_t packet = readEgtPacket(egtChannel);
	max_32855_code code = getResultCode(packet);
	if (code != MC_OK) {
		return EGT_ERROR_VALUE + code;
	} else {
		return GET_TEMPERATURE_C(packet);
	}
}

static void showEgtInfo() {
#if EFI_PROD_CODE
	printSpiState();

	efiPrintf("EGT spi: %d", engineConfiguration->max31855spiDevice);

	for (int i = 0; i < EGT_CHANNEL_COUNT; i++) {
		if (isBrainPinValid(engineConfiguration->max31855_cs[i])) {
			efiPrintf("%d ETG @ %s", i, hwPortname(engineConfiguration->max31855_cs[i]));
		}
	}
#endif
}

static void egtRead() {

	if (driver == NULL) {
		efiPrintf("No SPI selected for EGT");
		return;
	}

	efiPrintf("Reading egt");

	uint32_t egtPacket = readEgtPacket(0);

	max_32855_code code = getResultCode(egtPacket);

	efiPrintf("egt 0x%08x code=%d (%s)", egtPacket, code, getMcCode(code));

	if (code != MC_INVALID) {
		int refBits = ((egtPacket & 0xFFF0) >> 4); // bits 15:4
		float refTemp = refBits / 16.0;
		efiPrintf("reference temperature %.2f", refTemp);

		efiPrintf("EGT temperature %d", GET_TEMPERATURE_C(egtPacket));
	}
}

/* TODO: move all stuff to Max31855Read class */
class Max31855Read final : public ThreadController<UTILITY_THREAD_STACK_SIZE> {
public:
	Max31855Read()
		: ThreadController("MAX31855", MAX31855_PRIO)
	{
	}

	int start(spi_device_e device, egt_cs_array_t cs) {
		driver = getSpiDevice(device);

		if (driver) {
			/* WARN: this will clear all other bits in cr1 */
			spiConfig.cr1 = getSpiPrescaler(_5MHz, device);
			for (size_t i = 0; i < EGT_CHANNEL_COUNT; i++) {
				/*  and mark used! */
				if (isBrainPinValid(cs[i])) {
					initSpiCs(&spiConfig, cs[i]);
					m_cs[i] = cs[i];
				} else {
					m_cs[i] = Gpio::Invalid;
				}
			}
			ThreadController::start();
			return 0;
		}
		return -1;
	}

	void ThreadTask() override {
		while (true) {
			for (int i = 0; i < EGT_CHANNEL_COUNT; i++) {
			// todo: migrate to SensorType framework!
				engine->currentEgtValue[i] = getMax31855EgtValue(i);
			}

			chThdSleepMilliseconds(500);
		}
	}

private:
	//brain_pin_e m_cs[EGT_CHANNEL_COUNT];
};

static Max31855Read instance;

void initMax31855(spi_device_e device, egt_cs_array_t max31855_cs) {
	if (instance.start(device, max31855_cs) == 0) {
		addConsoleAction("egtinfo", (Void) showEgtInfo);
		addConsoleAction("egtread", (Void) egtRead);
	}
}

#endif /* EFI_MAX_31855 */
