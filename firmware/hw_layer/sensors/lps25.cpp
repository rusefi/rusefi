/**
 * @file        lps25.cpp
 * @brief       Driver for the ST LPS25HB pressure sensor
 *
 * @date February 6, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#include "lps25.h"

static constexpr uint8_t addr = 0x5C;
static constexpr uint8_t expectedWhoAmI = 0xBD;

// Control register 1
#define LPS_CR1_PD (1 << 7)
#define LPS_CR1_ODR_25hz (4 << 4)
#define LPS_CR1_BDU (1 << 2)

// Status register flags
#define LPS_SR_P_DA (1 << 1)	// Pressure data available

#define REG_WhoAmI 0x0F
#define REG_Cr1 0x20
#define REG_Status 0x27
#define REG_PressureOutXl 0x28
#define REG_PressureOutL 0x29
#define REG_PressureOutH 0x2A

bool Lps25::init(brain_pin_e scl, brain_pin_e sda) {
	if (!m_i2c.init(scl, sda)) {
		return false;
	}

	// Read ident register
	auto whoAmI = m_i2c.readRegister(addr, REG_WhoAmI);
	if (whoAmI != expectedWhoAmI) {
		return false;
	}

	// Set the control registers
	m_i2c.writeRegister(addr, REG_Cr1,
		LPS_CR1_PD |		// Set to active mode
		LPS_CR1_ODR_25hz |	// 25hz update rate

		// TODO: should bdu be set?
		LPS_CR1_BDU			// Output registers update only when read
	);

	m_hasInit = true;
	return true;
}

expected<float> Lps25::readPressureKpa() {
	if (!m_hasInit) {
		return unexpected;
	}

	// First read the status reg to check if there are data available
	uint8_t sr = m_i2c.readRegister(addr, REG_Status);

	bool hasPressure = sr & LPS_SR_P_DA;

	if (!hasPressure) {
		return unexpected;
	}

	/*
	TODO: why doesn't this work?

		// Sequential multi-byte reads need to set the high bit of the
		// register address to enable multi-byte read
		constexpr uint8_t readAddr = REG_PressureOutXl | 0x80;

		uint8_t buffer[3];
		m_i2c.writeRead(addr, &readAddr, 1, buffer, 3);

		// Glue the 3 bytes back in to a 24 bit integer
		uint32_t counts = buffer[2] << 16 | buffer[1] << 8 | buffer[0];
	*/

	auto xl = m_i2c.readRegister(addr, REG_PressureOutXl);
	auto l = m_i2c.readRegister(addr, REG_PressureOutL);
	auto h = m_i2c.readRegister(addr, REG_PressureOutH);

	// Glue the 3 bytes back in to a 24 bit integer
	uint32_t counts = h << 16 | l << 8 | xl;

	// 4096 counts per hectopascal
	// = 40960 counts per kilopascal
	constexpr float ratio = 1 / 40960.0f;
	
	float kilopascal = counts * ratio;

	// Sensor limits are 26-126 kPa
	// The highest ever barometric pressure measured was only 108.3 kPa
	// The pressure at the highest altitude road (Khardung La, India/Tibet) is at 5600 meters,
	// which should have a nominal barometric pressure of around 50 kPa
	// Anything outside that range is not a place we expect your engine to run, so we assume
	// some sensing problem (sealed ECU case and high temperature?)
	if (kilopascal > 120 || kilopascal < 50) {
		return unexpected;
	}

	return kilopascal;
}
