#include "pch.h"
#include "mcp4728.h"
#include "rusefi_lua.h"

#include <limits>
#include <vector>

class TestMcp4728 : public Mcp4728 {
public:
	bool initialize() { return initPins(Gpio::D7, Gpio::E8, Gpio::D5); }
	uint8_t address = 0;
	std::vector<uint8_t> bytes;
	std::vector<bool> bits;
	msg_t result = MSG_OK;
	int ackCount = 0;
	int nackAt = 0;
	int ldacFallsAfterAck = -1;
	size_t ldacFallsAfterBits = 0;
	bool ldacHigh = false;
	bool waited = false;
	bool locked = false;

	msg_t __write(uint8_t addr, const uint8_t* data, size_t size) override {
		EXPECT_TRUE(locked);
		address = addr;
		bytes.assign(data, data + size);
		return result;
	}
	msg_t lock() override { EXPECT_FALSE(locked); locked = true; return MSG_OK; }
	msg_t unlock() override { EXPECT_TRUE(locked); locked = false; return MSG_OK; }

protected:
	void sendBit(bool value) override { bits.push_back(value); }

	bool sda_get() override {
		EXPECT_TRUE(locked);
		++ackCount;
		EXPECT_EQ(ldacHigh, ackCount == 1);
		return ackCount == nackAt;
	}
	void setLdac(bool high) override {
		EXPECT_TRUE(locked);
		ldacHigh = high;
		if (!high) {
			ldacFallsAfterAck = ackCount;
			ldacFallsAfterBits = bits.size();
		}
	}
	void waitForEeprom() override { EXPECT_TRUE(locked); waited = true; }
};

TEST(Mcp4728, AllTwentyChannels) {
	TestMcp4728 dac;
	ASSERT_TRUE(dac.initialize());
	for (int channel = 1; channel <= 20; channel++) {
		ASSERT_TRUE(dac.setValue(channel, 0xabc));
		EXPECT_EQ(dac.address, 0x60 + (channel - 1) / 4);
		const std::vector<uint8_t> expected = {
			static_cast<uint8_t>(0x40 | (((channel - 1) % 4) << 1)), 0x0a, 0xbc
		};
		EXPECT_EQ(dac.bytes, expected);
		EXPECT_FALSE(dac.locked);
	}
}

TEST(Mcp4728, FourChannelsAndNack) {
	TestMcp4728 dac;
	ASSERT_TRUE(dac.initialize());
	const uint16_t values[] = {0, 1, 0x800, 0xfff};
	ASSERT_TRUE(dac.setChipValues(4, values));
	EXPECT_EQ(dac.address, 0x64);
	EXPECT_EQ(dac.bytes, (std::vector<uint8_t>{0x40, 0, 0, 0x42, 0, 1, 0x44, 8, 0, 0x46, 15, 255}));
	dac.result = MSG_RESET;
	EXPECT_FALSE(dac.setValue(20, 0));
	EXPECT_FALSE(dac.setChipValues(4, values));
	EXPECT_FALSE(dac.locked);
}

TEST(Mcp4728, VoltageConversion) {
	TestMcp4728 dac;
	ASSERT_TRUE(dac.initialize());
	ASSERT_TRUE(dac.setVoltage(1, 2.5f));
	EXPECT_EQ(dac.bytes, (std::vector<uint8_t>{0x40, 8, 0}));
	ASSERT_TRUE(dac.setVoltage(1, 0));
	EXPECT_EQ(dac.bytes, (std::vector<uint8_t>{0x40, 0, 0}));
	ASSERT_TRUE(dac.setVoltage(20, 3.3f, 3.3f));
	EXPECT_EQ(dac.bytes, (std::vector<uint8_t>{0x46, 15, 255}));
}

TEST(Mcp4728, RejectsInvalidInputsWithoutTraffic) {
	TestMcp4728 dac;
	const uint16_t values[] = {0, 1, 2, 3};
	EXPECT_FALSE(dac.setValue(1, 0));
	EXPECT_FALSE(dac.setChipValues(0, values));
	EXPECT_FALSE(dac.programAddress(1));
	EXPECT_FALSE(dac.initPins(Gpio::Unassigned, Gpio::E8, Gpio::D5));
	EXPECT_FALSE(dac.initPins(Gpio::D7, Gpio::D7, Gpio::D5));
	EXPECT_FALSE(dac.initPins(Gpio::D7, Gpio::E8, Gpio::D7));
	ASSERT_TRUE(dac.initialize());
	EXPECT_TRUE(dac.initialize());
	EXPECT_FALSE(dac.initPins(Gpio::A0, Gpio::A1, Gpio::A2));
	EXPECT_FALSE(dac.setValue(0, 0));
	EXPECT_FALSE(dac.setValue(21, 0));
	EXPECT_FALSE(dac.setValue(1, -1));
	EXPECT_FALSE(dac.setValue(1, 4096));
	EXPECT_FALSE(dac.setChipValues(-1, values));
	EXPECT_FALSE(dac.setChipValues(5, values));
	const uint16_t badValues[] = {0, 1, 2, 4096};
	EXPECT_FALSE(dac.setChipValues(0, badValues));
	EXPECT_FALSE(dac.programAddress(-1));
	EXPECT_FALSE(dac.programAddress(8));
	EXPECT_FALSE(dac.setVoltage(1, -1));
	EXPECT_FALSE(dac.setVoltage(1, 6));
	EXPECT_FALSE(dac.setVoltage(1, 0, 0));
	EXPECT_FALSE(dac.setVoltage(1, std::numeric_limits<float>::quiet_NaN()));
	EXPECT_FALSE(dac.setVoltage(1, std::numeric_limits<float>::infinity()));
	EXPECT_FALSE(dac.setVoltage(1, 1, std::numeric_limits<float>::infinity()));
	EXPECT_TRUE(dac.bytes.empty());
	EXPECT_EQ(dac.ackCount, 0);
}

TEST(Mcp4728, AddressProgrammingLatchAndEepromWait) {
	for (int address = 0; address <= 7; address++) {
		TestMcp4728 dac;
		ASSERT_TRUE(dac.initialize());
		EXPECT_TRUE(dac.programAddress(address));
		EXPECT_EQ(dac.ackCount, 4);
		EXPECT_EQ(dac.ldacFallsAfterAck, 1);
		EXPECT_EQ(dac.ldacFallsAfterBits, 16u);
		ASSERT_EQ(dac.bits.size(), 32u);
		const uint8_t expected[] = {0xc0, 0x61,
			static_cast<uint8_t>(0x62 | (address << 2)),
			static_cast<uint8_t>(0x63 | (address << 2))};
		for (size_t bit = 0; bit < dac.bits.size(); bit++) {
			EXPECT_EQ(dac.bits[bit], (expected[bit / 8] & (0x80 >> (bit % 8))) != 0);
		}
		EXPECT_TRUE(dac.waited);
		EXPECT_FALSE(dac.locked);
	}
}

TEST(Mcp4728, AddressProgrammingAbortsOnEachNack) {
	for (int byte = 1; byte <= 4; byte++) {
		TestMcp4728 dac;
		ASSERT_TRUE(dac.initialize());
		dac.nackAt = byte;
		EXPECT_FALSE(dac.programAddress(4));
		EXPECT_EQ(dac.ackCount, byte);
		EXPECT_FALSE(dac.waited);
		EXPECT_FALSE(dac.locked);
	}
}

TEST(Mcp4728, LuaValidationAndUninitializedBus) {
	EXPECT_EQ(testLuaReturnsNumber(R"(
		function testFunc()
			return (setI2cDac(20, 4095) == false
				and setI2cDacVoltage(1, 2.5) == false
				and setI2cDacChannels(4, 0, 1, 2048, 4095) == false) and 1 or 0
		end
	)"), 1);
	for (const char* call : {
		"setI2cDac(0, 0)", "setI2cDac(21, 0)", "setI2cDac(1, -1)",
		"setI2cDac(1, 4096)", "setI2cDac(1, 1.5)",
		"setI2cDacVoltage(1, 6)", "setI2cDacVoltage(1, 0/0)",
		"setI2cDacVoltage(1, 0, 0)", "setI2cDacVoltage(1, 0, 1/0)",
		"setI2cDacChannels(5, 0, 0, 0, 0)", "setI2cDacChannels(0, 0, 0, 0, 4096)"
	}) {
		const auto script = std::string("function testFunc() ") + call + " return 1 end";
		EXPECT_THROW(testLuaReturnsNumber(script.c_str()), std::logic_error) << call;
	}
}
