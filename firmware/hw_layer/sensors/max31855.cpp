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

// bits D17 and D3 are always expected to be zero
#define MC_RESERVED_BITS 0x20008
#define MC_OPEN_BIT 1
#define MC_GND_BIT 2
#define MC_VCC_BIT 4

/* TODO: move all stuff to Max31855Read class */
class Max31855Read final : public ThreadController<UTILITY_THREAD_STACK_SIZE> {
public:
	Max31855Read()
		: ThreadController("MAX31855", MAX31855_PRIO)
	{
	}

	typedef enum {
		MC_OK = 0,
		MC_INVALID = 1,
		MC_OPEN = 2,
		MC_SHORT_GND = 3,
		MC_SHORT_VCC = 4,
	} max_31855_code;

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
				float value;

				max_31855_code ret = getMax31855EgtValue(i, &value, NULL);
				if (ret == MC_OK) {
					// todo: migrate to SensorType framework!
					engine->currentEgtValue[i] = value;
				} else {
					/* TODO: report error code? */
				}
			}

			chThdSleepMilliseconds(500);
		}
	}

	/* Debug stuff */
	void showEgtInfo() {
	#if EFI_PROD_CODE
		printSpiState();

		efiPrintf("EGT spi: %d", engineConfiguration->max31855spiDevice);

		for (int i = 0; i < EGT_CHANNEL_COUNT; i++) {
			if (isBrainPinValid(m_cs[i])) {
				efiPrintf("%d ETG @ %s", i, hwPortname(m_cs[i]));
			}
		}
	#endif
	}

	void egtRead() {
		float temp, refTemp;

		if (driver == NULL) {
			efiPrintf("No SPI selected for EGT");
			return;
		}

		efiPrintf("Reading egt");

		max_31855_code code = getMax31855EgtValue(0, &temp, &refTemp);

		efiPrintf("egt: code=%d (%s)", code, getMcCode(code));

		if (code != MC_INVALID) {
			efiPrintf("reference temperature %.2f", refTemp);
			efiPrintf("EGT temperature %d", temp);
		}
	}

private:
	brain_pin_e m_cs[EGT_CHANNEL_COUNT];

	SPIDriver *driver;

	/* TODO: validate */
	SPIConfig spiConfig = {
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

	max_31855_code getResultCode(uint32_t egtPacket) {
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

	const char * getMcCode(max_31855_code code) {
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

	int readEgtPacket(size_t egtChannel, uint32_t *packet) {
		uint32_t egtPacket;
		brain_pin_e cs = m_cs[egtChannel];

		if ((!isBrainPinValid(cs)) || (driver == NULL)) {
			return -1;
		}

		/* Set proper CS gpio */
		initSpiCsNoOccupy(&spiConfig, cs);

		spiStart(driver, &spiConfig);
		spiSelect(driver);

		spiReceive(driver, sizeof(egtPacket), &egtPacket);

		spiUnselect(driver);
		spiStop(driver);

		*packet = SWAP_UINT32(egtPacket);

		return 0;
	}

	float packetGetTemperature(uint32_t packet) {
		return ((float)(packet >> 18) / 4.0);
	}

	float packetGetRefTemperature(uint32_t packet) {
		// bits 15:4
		return (float)((packet & 0xFFF0) >> 4) / 16.0;
	}

	max_31855_code getMax31855EgtValue(size_t egtChannel, float *temp, float *refTemp) {
		uint32_t packet;
		max_31855_code code = MC_INVALID;
		int ret;

		ret = readEgtPacket(egtChannel, &packet);
		if (ret == 0) {
			code = getResultCode(packet);
		}

		if (code == MC_OK) {
			if (temp) {
				*temp = packetGetTemperature(packet);
			}
			if (refTemp) {
				*refTemp = packetGetRefTemperature(packet);
			}
		}
		return code;
	}
};

static Max31855Read instance;

void showEgtInfo() {
	instance.showEgtInfo();
}

void egtRead() {
	instance.egtRead();
}

void initMax31855(spi_device_e device, egt_cs_array_t max31855_cs) {
	if (instance.start(device, max31855_cs) == 0) {
		addConsoleAction("egtinfo", (Void) showEgtInfo);
		addConsoleAction("egtread", (Void) egtRead);
	}
}

#endif /* EFI_MAX_31855 */
