#include "pch.h"

#include "ads1015.h"

constexpr uint8_t addr = 0x48;

#define ADS1015_CONV (0)
#define ADS1015_CONFIG (1)
#define ADS1015_LO_THRESH (2)
#define ADS1015_HI_THRESH (3)

bool Ads1015::init(brain_pin_e scl, brain_pin_e sda) {
	if (!m_i2c.init(scl, sda)) {
		return false;
	}

	// ADS1015 has no ID register - so we read the Lo_thresh instead
	uint16_t loThresh = readReg(ADS1015_LO_THRESH);

	if (loThresh != 0x8000) {
		return false;
	}

	m_hasInit = true;
	return true;
}

void Ads1015::readChannels(float (&result)[4]) {
	for (size_t i = 0; i < 4; i++) {
		result[i] = readChannel(i);
	}
}

float Ads1015::readChannel(uint8_t ch) {
	// set the channel
	// set to +-6.144v full scale, fastest sampling, manual conversion start
	writeReg(ADS1015_CONFIG, 0xC1E0 | ch << 12);

	// Wait for conversion to complete
	// Bit is cleared while conversion is ongoing, set when done
	while ((readReg(ADS1015_CONFIG) & 0x8000) == 0) ;

	// Read the result
	int16_t result = readReg(ADS1015_CONV);
	// Result is 12 bits left aligned, so right align the result
	result = result >> 4;

	// 2048 counts = positive 6.144 volts
	constexpr float ratio = 6.144f / 2048;
	return result * ratio;
}

void Ads1015::writeReg(uint8_t reg, uint16_t data) {
	uint8_t packet[3];

	packet[0] = reg;
	packet[1] = data >> 8;
	packet[2] = data & 0xFF;

	m_i2c.write(addr, packet, 3);
}

uint16_t Ads1015::readReg(uint8_t reg) {
	uint16_t res;
	m_i2c.write(addr, &reg, 1);
	m_i2c.read(addr, reinterpret_cast<uint8_t*>(&res), 2);
	return SWAP_UINT16(res);
}
