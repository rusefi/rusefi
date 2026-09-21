/**
 * @file        lps25.h
 * @brief       Driver for the ST LPS22HB and LPS25HB pressure sensor
 *
 * @date February 6, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#pragma once

#include "i2c.h"

class Lps25 : public StoredValueSensor {
public:
	Lps25() : StoredValueSensor(SensorType::BarometricPressure, MS2NT(1000)) { }

	void showInfo(const char* sensorName) const override;

	// Returns true if the sensor was initialized successfully.
	bool init(brain_pin_e scl, brain_pin_e sda);

	expected<float> readPressureKpa();
	bool hasInit() const {
		return m_hasInit;
	}

private:
	i2cBus *m_i2c;

	uint8_t readRegister(uint8_t reg);
	void writeRegister(uint8_t reg, uint8_t val);

	enum class Type {
		Lps22,
		Lps25,
	};

	Type m_type;

	bool m_hasInit = false;

	float temperature;

	uint8_t regCr1() const;
};

#if EFI_PROD_CODE
#include "periodic_thread_controller.h"

class Lps25Thread : public PeriodicController<UTILITY_THREAD_STACK_SIZE> {
public:
	Lps25Thread(Lps25 &driver) :
		PeriodicController("LPS25"),
		m_driver(driver) { }

	// Returns true if the sensor was initialized successfully.
	bool init();

	void PeriodicTask(efitick_t nowNt) override;
private:
	Lps25 &m_driver;
};
#endif /* EFI_PROD_CODE */
