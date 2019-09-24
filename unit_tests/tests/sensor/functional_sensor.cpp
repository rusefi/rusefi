#include "functional_sensor.h"

#include <gtest/gtest.h>

struct DoublerFunc final : public SensorConverter {
	SensorResult convert(float input) const {
		bool valid = input > 0;
		float value = input * 2;

		return {valid, value};
	}
};

class SensorConverted : public ::testing::Test {
protected:
	SensorConverted() : dut(SensorType::Clt) { }

	void SetUp() override {
		Sensor::resetRegistry();
	}

	void TearDown() override {
		Sensor::resetRegistry();
	}

	FunctionalSensor<DoublerFunc> dut;
};

TEST_F(SensorConverted, TestValid) {
	ASSERT_TRUE(dut.Register());

	// Should be invalid - not set yet
	{
		auto s = Sensor::get(SensorType::Clt);
		EXPECT_FALSE(s.Valid);
	}

	dut.postRawValue(25);

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

	dut.postRawValue(-25);

	// Should be invalid, with a value of -25*2 = 0
	{
		auto s = Sensor::get(SensorType::Clt);
		EXPECT_FALSE(s.Valid);
		EXPECT_FLOAT_EQ(s.Value, 0);
	}
}

TEST_F(SensorConverted, TestGet) {
	// we're only checking that this compiles
	DoublerFunc& f = dut.f();
}