#include "pch.h"
#include "lps25.h"

class Lps25TestBus : public i2cBus {
public:
	bool init(brain_pin_e scl, brain_pin_e sda, i2c_speed_e) override { return true; }

	void deinit() override { }
	msg_t __write(uint8_t, const uint8_t*, size_t) override { return MSG_OK; }
	msg_t __read(uint8_t, uint8_t*, size_t) override { return MSG_RESET; }
	msg_t __writeRead(uint8_t address, const uint8_t* writeData, size_t writeSize,
		uint8_t* readData, size_t readSize) override {
		if (address == 0x5c && writeSize == 1 && readSize == 1 && writeData[0] == 0x0f) {
			readData[0] = 0xbd;
			return MSG_OK;
		}
		if (address == 0x5c && writeSize == 1 && readSize == 6 && writeData[0] == 0xa7) {
			readData[0] = 0x02; // Pressure data available
			readData[1] = 0x00;
			readData[2] = 0x80;
			readData[3] = 0x3e; // 100 kPa at 40960 counts/kPa
			readData[4] = 0;
			readData[5] = 0;
			return MSG_OK;
		}
		return MSG_RESET;
	}
	msg_t lock() override { return MSG_OK; }
	msg_t unlock() override { return MSG_OK; }
};

TEST(Lps25, LegacyPinsWorkWithoutSharedBus) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	Lps25TestBus fallback;
	Lps25 baro(&fallback);

	// Saved uaEFI tunes contain the barometer pins but no enabled shared I2C bus.
	// getI2cBus should fallback and start one of I2C_BUS_LEGACY_COUNT bus in BB mode
	ASSERT_NE(nullptr, getI2cBus(Gpio::B10, Gpio::B11));

	ASSERT_TRUE(baro.init(Gpio::B10, Gpio::B11));
	ASSERT_TRUE(Sensor::hasSensor(SensorType::BarometricPressure));
	auto pressure = baro.readPressureKpa();
	ASSERT_TRUE(pressure);
	EXPECT_NEAR(100, pressure.Value, 0.01f);
	EXPECT_NEAR(100, Sensor::getOrZero(SensorType::BarometricPressure), 0.01f);
	Sensor::resetRegistry();
}
