/**
 * @file max3185x.cpp
 * @brief MAX31855 and MAX31856 Thermocouple-to-Digital Converter driver
 *
 *
 * http://datasheets.maximintegrated.com/en/ds/MAX31855.pdf
 * https://www.analog.com/media/en/technical-documentation/data-sheets/MAX31856.pdf
 *
 *
 * Read-only (MAX31855), RW (MAX31956) communication over 5MHz SPI
 *
 * @date Sep 17, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 * @author Andrey Gusakov, 2024
 *
 */

#include "pch.h"
#include "max3185x.h"

#include "hardware.h"

#if EFI_PROD_CODE
#include "mpu_util.h"
#endif /* EFI_PROD_CODE */

#if EFI_MAX_31855

#include "periodic_task.h"
#include "stored_value_sensor.h"

#ifndef MAX3185X_REFRESH_TIME
#define MAX3185X_REFRESH_TIME 100
#endif

/* TODO: move all stuff to Max3185xRead class */
class Max3185xRead final : public PeriodicTimerController {
public:
	typedef enum {
		UNKNOWN_TYPE = 0,
		MAX31855_TYPE = 1,
		MAX31856_TYPE = 2,
	} Max3185xType;

	typedef enum {
		MAX3185X_OK = 0,
		MAX3185X_OPEN_CIRCUIT = 1,
		MAX3185X_SHORT_TO_GND = 2,
		MAX3185X_SHORT_TO_VCC = 3,
		MAX3185X_NO_REPLY = 4,
		MAX3185X_NOT_ENABLED = 5,
	} Max3185xState;

	int start(spi_device_e device, egt_cs_array_t cs) {
		driver = getSpiDevice(device);

		if (driver) {
			/* WARN: this will clear all other bits in cr1 */
			//spiConfig.cr1 = getSpiPrescaler(_5MHz, device);
			for (size_t i = 0; i < EGT_CHANNEL_COUNT; i++) {
				auto& sensor = egtSensors[i];

				m_cs[i] = Gpio::Invalid;
				types[i] = UNKNOWN_TYPE;

				// If there's already another (CAN?) EGT sensor configured,
				// don't configure this one.
				if (Sensor::hasSensor(sensor.type()))
					continue;

				// get CS pin and mark used!
				if (isBrainPinValid(cs[i])) {
					initSpiCs(&spiConfig, cs[i]);
					m_cs[i] = cs[i];

					sensor.Register();
				}
			}
			PeriodicTimerController::start();
			return 0;
		}

		efiPrintf("EGT not configured");
		return -1;
	}

	void stop(void) {
		PeriodicTimerController::stop();

		for (size_t i = 0; i < EGT_CHANNEL_COUNT; i++) {
			if (!isBrainPinValid(m_cs[i])) {
				continue;
			}

			auto& sensor = egtSensors[i];

			brain_pin_markUnused(m_cs[i]);
			sensor.unregister();
		}
	}

	void PeriodicTask() override {
		for (int i = 0; i < EGT_CHANNEL_COUNT; i++) {
			float value;

			Max3185xState ret = getMax3185xEgtValues(i, &value, NULL);
			if (ret == MAX3185X_OK) {
				auto& sensor = egtSensors[i];

				sensor.setValidValue(value, getTimeNowNt());
			} else {
				/* TODO: report error code? */
			}
		}
	}

	int getPeriodMs() override {
		return 500;
	}

	/* Debug stuff */
	void showEgtInfo() {
	#if EFI_PROD_CODE
		printSpiState();

		efiPrintf("EGT spi: %d", engineConfiguration->max31855spiDevice);

		for (int i = 0; i < EGT_CHANNEL_COUNT; i++) {
			if (isBrainPinValid(m_cs[i])) {
				efiPrintf("EGT CS %d @%s", i + 1, hwPortname(m_cs[i]));
			}
		}
	#endif
	}

	void egtRead() {
		if (driver == NULL) {
			efiPrintf("No SPI selected for EGT");
			return;
		}

		efiPrintf("Reading egt(s)");

		for (size_t i = 0; i < EGT_CHANNEL_COUNT; i++) {
			float temp, refTemp;
			Max3185xState code = getMax3185xEgtValues(i, &temp, &refTemp);

			efiPrintf("egt%d: type %s, code=%d (%s)", i + 1, getMax3185xTypeName(types[i]), code, getMax3185xErrorCodeName(code));

			if (code == MAX3185X_OK) {
				efiPrintf(" temperature %.4f reference temperature %.2f", temp, refTemp);
			}
		}
	}

private:
	// bits D17 and D3 are always expected to be zero
	#define MAX31855_RESERVED_BITS	0x20008

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
			((5 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |	/* div = 64 */
			SPI_CR1_MSTR |
			/* SPI_CR1_CPOL | */ // = 0
			SPI_CR1_CPHA | // = 1
			0,
		.cr2 = SPI_CR2_8BIT_MODE
	};

	const char * getMax3185xErrorCodeName(Max3185xState code) {
		switch (code) {
		case MAX3185X_OK:
			return "Ok";
		case MAX3185X_OPEN_CIRCUIT:
			return "Open";
		case MAX3185X_SHORT_TO_GND:
			return "short gnd";
		case MAX3185X_SHORT_TO_VCC:
			return "short VCC";
		case MAX3185X_NO_REPLY:
			return "no reply";
		case MAX3185X_NOT_ENABLED:
			return "not enabled";
		default:
			return "invalid";
		}
	}

	const char *getMax3185xTypeName(Max3185xType type) {
		switch (type) {
		case MAX31855_TYPE:
			return "max31855";
		case MAX31856_TYPE:
			return "max31856";
		default:
			return "unknown";
		}
	}

	int spi_txrx(size_t channel, uint8_t tx[], uint8_t rx[], size_t n)
	{
		brain_pin_e cs = m_cs[channel];

		if ((!isBrainPinValid(cs)) || (driver == NULL)) {
			return -1;
		}

		/* Set proper CS gpio */
		initSpiCsNoOccupy(&spiConfig, cs);

		/* Acquire ownership of the bus. */
		spiAcquireBus(driver);
		/* Setup transfer parameters. */
		spiStart(driver, &spiConfig);
		/* Slave Select assertion. */
		spiSelect(driver);
		spiExchange(driver, n, tx, rx);
		/* Slave Select de-assertion. */
		spiUnselect(driver);
		/* Bus deinit */
		spiStop(driver);
		/* Ownership release. */
		spiReleaseBus(driver);

		/* no errors for now */
		return 0;
	}

	int spi_rx32(size_t channel, uint32_t *data)
	{
		int ret;
		/* dummy */
		uint8_t tx[4] = {0};
		uint8_t rx[4];

		ret = spi_txrx(channel, tx, rx, 4);
		if (ret) {
			return ret;
		}
		if (data) {
			*data =	(rx[0] << 24) |
					(rx[1] << 16) |
					(rx[2] <<  8) |
					(rx[3] <<  0);
		}

		return 0;
	}

	Max3185xType detect(size_t channel)
	{
		int ret;
		uint8_t rx[4];
		uint8_t tx[4];

		/* try to apply settings to max31956 and then read back settings */
		// Wr, register 0x00
		tx[0] = 0x00 | BIT(7);
		// CR0: 50Hz mode
		// Change the notch frequency only while in the "Normally Off" mode - not in the Automatic
		tx[1] = 0x01;
		ret = spi_txrx(channel, tx, rx, 2);
		if (ret) {
			return UNKNOWN_TYPE;
		}

		// CR0: Automatic Conversion mode, OCFAULT = 2, 50Hz mode
		tx[1] = BIT(7) | BIT(0) | (2 << 4);
		// CR1: 4 samples average, K type
		tx[2] = (2 << 4) | (3 << 0);
		ret = spi_txrx(channel, tx, rx, 3);
		if (ret) {
			return UNKNOWN_TYPE;
		}

		/* Now readback settings */
		tx[0] = 0x00;
		ret = spi_txrx(channel, tx, rx, 4);
		if ((rx[1] == tx[1]) && (rx[2] == tx[2])) {
			return MAX31856_TYPE;
		}

		/* in case of max31855 we get standart reply with few reserved, always zero bits */
		uint32_t data = (rx[0] << 24) |
						(rx[1] << 16) |
						(rx[2] <<  8) |
						(rx[3] <<  0);

		/* MISO is constantly low or high */
		if ((data == 0xffffffff) || (data == 0x0)) {
			return UNKNOWN_TYPE;
		}

		if ((data & MAX31855_RESERVED_BITS) == 0x0) {
			return MAX31855_TYPE;
		}

		return UNKNOWN_TYPE;
	}

	Max3185xState getMax31855ErrorCode(uint32_t egtPacket) {
		#define MAX33855_FAULT_BIT			BIT(16)
		#define MAX33855_OPEN_BIT			BIT(0)
		#define MAX33855_GND_BIT			BIT(1)
		#define MAX33855_VCC_BIT			BIT(2)

		if (((egtPacket & MAX31855_RESERVED_BITS) != 0) ||
			(egtPacket == 0x0) ||
			(egtPacket == 0xffffffff)) {
			return MAX3185X_NO_REPLY;
		} else if ((egtPacket & MAX33855_OPEN_BIT) != 0) {
			return MAX3185X_OPEN_CIRCUIT;
		} else if ((egtPacket & MAX33855_GND_BIT) != 0) {
			return MAX3185X_SHORT_TO_GND;
		} else if ((egtPacket & MAX33855_VCC_BIT) != 0) {
			return MAX3185X_SHORT_TO_VCC;
		} else {
			return MAX3185X_OK;
		}
	}

	Max3185xState getMax31855EgtValues(size_t channel, float *temp, float *coldJunctionTemp) {
		uint32_t packet;
		Max3185xState code = MAX3185X_NO_REPLY;
		int ret;

		ret = spi_rx32(channel, &packet);
		if (ret == 0) {
			code = getMax31855ErrorCode(packet);
		}

		if (code != MAX3185X_OK) {
			return code;
		}

		if (temp) {
			// bits 31:18, 0.25C resolution (1/4 C)
			int16_t tmp = (packet >> 18) & 0x3fff;
			/* extend sign */
			tmp = tmp << 2;
			tmp = tmp >> 2;	/* shifting right signed is not a good idea */
			*temp = (float) tmp * 0.25;
		}
		if (coldJunctionTemp) {
			// bits 15:4, 0.0625C resolution (1/16 C)
			int16_t tmp = (packet >> 4) & 0xfff;
			/* extend sign */
			tmp = tmp << 4;
			tmp = tmp >> 4;	/* shifting right signed is not a good idea */
			*coldJunctionTemp = (float)tmp * 0.0625;
		}

		return code;
	}

	Max3185xState getMax31856EgtValues(size_t channel, float *temp, float *coldJunctionTemp)
	{
		uint8_t rx[7];
		/* read Cold-Junction temperature MSB, LSB, Linearized TC temperature 3 bytes and Fault Status */
		uint8_t tx[7] = {0x0a};

		int ret = spi_txrx(channel, tx, rx, 7);
		if (ret) {
			return MAX3185X_NO_REPLY;
		}

		if (rx[6] & BIT(0)) {
			return MAX3185X_OPEN_CIRCUIT;
		} else if (rx[6] & BIT(1)) {
			return MAX3185X_SHORT_TO_VCC;
		}

		if (temp) {
			// 10 bit before point and 7 bits after
			int32_t tmp = (rx[3] << 11) | (rx[4] << 3) | (rx[5] >> 5);
			/* extend sign: move top bit 18 to 31 */
			tmp = tmp << 13;
			tmp = tmp >> 13;	/* shifting right signed is not a good idea */
			*temp = ((float)tmp) / 128.0;
		}

		/* convert float to int */
		if (coldJunctionTemp) {
			int16_t tmp = (rx[1] << 6) | (rx[2] >> 2);
			/* extend sign */
			tmp = tmp << 2;
			tmp = tmp >> 2;	/* shifting right signed is not a good idea */
			*coldJunctionTemp = ((float)tmp) / 64.0;
		}

		return MAX3185X_OK;
	}

	Max3185xState getMax3185xEgtValues(size_t channel, float *temp, float *coldJunctionTemp) {
		Max3185xState ret;

		if ((!isBrainPinValid(m_cs[channel])) || (!driver)) {
			return MAX3185X_NOT_ENABLED;
		}

		/* if chip type is not detected yet try to detect */
		if (types[channel] == UNKNOWN_TYPE) {
			types[channel] = detect(channel);
		}

		/* failed? bail out */
		if (types[channel] == UNKNOWN_TYPE) {
			return MAX3185X_NO_REPLY;
		}

		if (types[channel] == MAX31855_TYPE) {
			ret = getMax31855EgtValues(channel, temp, coldJunctionTemp);
		} else {
			ret = getMax31856EgtValues(channel, temp, coldJunctionTemp);
		}

		if (ret == MAX3185X_NO_REPLY) {
			types[channel] = UNKNOWN_TYPE;
		}

		return ret;
	}

	Max3185xType types[EGT_CHANNEL_COUNT];

	StoredValueSensor egtSensors[EGT_CHANNEL_COUNT] = {
		{ SensorType::EGT1, MS2NT(MAX3185X_REFRESH_TIME * 3) },
		{ SensorType::EGT2, MS2NT(MAX3185X_REFRESH_TIME * 3) },
		{ SensorType::EGT3, MS2NT(MAX3185X_REFRESH_TIME * 3) },
		{ SensorType::EGT4, MS2NT(MAX3185X_REFRESH_TIME * 3) },
		{ SensorType::EGT5, MS2NT(MAX3185X_REFRESH_TIME * 3) },
		{ SensorType::EGT6, MS2NT(MAX3185X_REFRESH_TIME * 3) },
		{ SensorType::EGT7, MS2NT(MAX3185X_REFRESH_TIME * 3) },
		{ SensorType::EGT8, MS2NT(MAX3185X_REFRESH_TIME * 3) }
	};
};

static Max3185xRead instance;

static void showEgtInfo() {
	instance.showEgtInfo();
}

static void egtRead() {
	instance.egtRead();
}

void initMax3185x(spi_device_e device, egt_cs_array_t max31855_cs) {
	addConsoleAction("egtinfo", (Void) showEgtInfo);
	addConsoleAction("egtread", (Void) egtRead);

	startMax3185x(device, max31855_cs);
}

void stopMax3185x(void)
{
	instance.stop();
}

void startMax3185x(spi_device_e device, egt_cs_array_t max31855_cs)
{
	instance.start(device, max31855_cs);
}

#endif /* EFI_MAX_31855 */
