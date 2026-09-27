#pragma once

#include "i2c_bb.h"

#if LUA_I2C_DAC

#include <atomic>

// todo: migrate to i2cBus *getI2cBus(brain_pin_e scl, brain_pin_e sda) for extra flexibility?
// Five MCP4728s at 7-bit addresses 0x60..0x64, four outputs per chip.
class Mcp4728 : public BitbangI2c {
public:
	static constexpr int ChipCount = 5;
	static constexpr int ChannelCount = 4 * ChipCount;
	static constexpr int MaxValue = 4095;

	bool initPins(brain_pin_e scl, brain_pin_e sda, brain_pin_e ldac);
	// Channels are 1-based. Writes only touch volatile registers, using VDD as reference.
	bool setValue(int channel, int value);
	bool setVoltage(int channel, float voltage, float vcc = 5.0f);
	bool setChipValues(int address, const uint16_t (&values)[4]);
	// One-time EEPROM programming of a chip at factory address 0.
	bool programAddress(int address);

protected:
	void waitQuarterBit() override;
	virtual void setLdac(bool high);
	virtual void waitForEeprom();

private:
	// Published by the Lua thread, also read by the console thread.
	std::atomic<bool> m_initialized{false};
	brain_pin_e m_ldac = Gpio::Unassigned;
};

Mcp4728& getLuaI2cDac();
void initI2cDacConsole();

#endif // LUA_I2C_DAC
