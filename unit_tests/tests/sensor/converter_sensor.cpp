#include "converter_sensor.h"

#include <gtest/gtest.h>

class SensorConverted : public ::testing::Test
{
protected:
	void SetUp() override
	{
		Sensor::resetRegistry();
	}

	void TearDown() override
	{
		Sensor::resetRegistry();
	}
};

class DoublerConverterSensor final : public ConvertedSensor
{
public:
	DoublerConverterSensor() : ConvertedSensor(SensorType::Clt) {}

protected:
	SensorResult convertFromInputValue(float input)
	{
		bool valid = input > 0;
		float value = input * 2;

		return { valid, value };
	}
};

TEST_F(SensorConverted, TestValid)
{
	DoublerConverterSensor dut;
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

TEST_F(SensorConverted, TestInvalid)
{
	DoublerConverterSensor dut;
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
