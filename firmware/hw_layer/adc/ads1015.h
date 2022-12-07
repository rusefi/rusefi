#pragma once

#include "i2c_bb.h"

class Ads1015 {
public:
	bool init(brain_pin_e scl, brain_pin_e sda);
	void readChannels(float (&result)[4]);

private:
	float readChannel(uint8_t ch);

	void writeReg(uint8_t reg, uint16_t data);
	uint16_t readReg(uint8_t reg);

	bool m_hasInit = false;
	BitbangI2c m_i2c;
};
