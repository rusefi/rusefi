/**
 * @file        lps25.h
 * @brief       Driver for the ST LPS22HB and LPS25HB pressure sensor
 *
 * @date February 6, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#pragma once

#include "i2c_bb.h"

class Lps25 {
public:
	// Returns true if the sensor was initialized successfully.
	bool init(brain_pin_e scl, brain_pin_e sda);

	expected<float> readPressureKpa();
	bool hasInit() const {
		return m_hasInit;
	}

private:
	BitbangI2c m_i2c;

	enum class Type {
		Lps22,
		Lps25,
	};

	Type m_type;

	bool m_hasInit = false;

	uint8_t regCr1() const;
};
