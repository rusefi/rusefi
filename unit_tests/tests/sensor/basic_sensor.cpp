#include "stored_value_sensor.h"

#include <gtest/gtest.h>

#include "mock/mock_sensor.h"

class SensorBasic : public ::testing::Test
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

TEST_F(SensorBasic, Registry)
{
	// Create a sensor - but don't register it
	MockSensor dut(SensorType::Tps1);

	// Expect not to find it
	{
		auto s = Sensor::getSensorOfType(SensorType::Tps1);
		EXPECT_FALSE(s);
	}

	// Register the sensor now
	EXPECT_TRUE(dut.Register());

	// It should now get us back our sensor
	{
		auto s = Sensor::getSensorOfType(SensorType::Tps1);
		EXPECT_EQ(s, &dut);
	}

	// Reset - it should now be gone!
	Sensor::resetRegistry();

	{
		auto s = Sensor::getSensorOfType(SensorType::Tps1);
		EXPECT_FALSE(s);
	}
}

TEST_F(SensorBasic, DoubleRegister)
{
	// Create a sensor, register it
	MockSensor dut(SensorType::Tps1);
	ASSERT_TRUE(dut.Register());

	// And then do it again!
	MockSensor dut2(SensorType::Tps1);
	EXPECT_FALSE(dut2.Register());

	// Make sure that we get the first DUT back - not the second
	auto shouldBeDut = Sensor::getSensorOfType(SensorType::Tps1);
	EXPECT_EQ(shouldBeDut, &dut);
	EXPECT_NE(shouldBeDut, &dut2);
}

TEST_F(SensorBasic, SensorNotInitialized)
{
	auto result = Sensor::get(SensorType::Clt);

	EXPECT_FALSE(result.Valid);
}

TEST_F(SensorBasic, SensorInitialized)
{
	MockSensor dut(SensorType::Clt);
	ASSERT_TRUE(dut.Register());

	// Check before init - make sure it isn't set yet
	auto result = Sensor::get(SensorType::Clt);
	ASSERT_FALSE(result.Valid);

	// Set a value
	dut.set(75);

	// Make sure now it's set
	auto result2 = Sensor::get(SensorType::Clt);
	EXPECT_TRUE(result2.Valid);
	EXPECT_FLOAT_EQ(result2.Value, 75);
}
