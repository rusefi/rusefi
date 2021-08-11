#include "pch.h"

#include "functional_sensor.h"

struct DoublerFunc final : public SensorConverter {
	SensorResult convert(float input) const {
		if (input <= 0) {
			return unexpected;
		}

		return input * 2;
	}
};

class SensorConverted : public ::testing::Test {
protected:
	SensorConverted()
		: dut(SensorType::Clt, MS2NT(50)) {}

	void SetUp() override {
		dut.setFunction(func);
		Sensor::resetRegistry();
	}

	void TearDown() override {
		Sensor::resetRegistry();
	}

	FunctionalSensor dut;
	DoublerFunc func;
};

TEST_F(SensorConverted, TestValid) {
	ASSERT_TRUE(dut.Register());

	// Should be invalid - not set yet
	{
		auto s = Sensor::get(SensorType::Clt);
		EXPECT_FALSE(s.Valid);
	}

	dut.postRawValue(25, getTimeNowNt());

	// Should be valid, with a value of 25*2 = 50
	{
		auto s = Sensor::get(SensorType::Clt);
		EXPECT_TRUE(s.Valid);
		EXPECT_FLOAT_EQ(s.Value, 50);
	}
}

TEST_F(SensorConverted, TestInvalid) {
	ASSERT_TRUE(dut.Register());

	// Should be invalid - not set yet
	{
		auto s = Sensor::get(SensorType::Clt);
		EXPECT_FALSE(s.Valid);
	}

	dut.postRawValue(-25, 0);

	// Should be invalid, with a value of -25*2 = 0
	{
		auto s = Sensor::get(SensorType::Clt);
		EXPECT_FALSE(s.Valid);
		EXPECT_FLOAT_EQ(s.Value, 0);
	}
}
