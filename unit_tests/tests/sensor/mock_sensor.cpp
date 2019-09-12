#include "sensor.h"

#include <gtest/gtest.h>

#include "mock/mock_sensor.h"

class SensorMocking : public ::testing::Test
{
protected:
	MockSensor realSensor;

	SensorMocking() : realSensor(SensorType::Clt) { }

	void SetUp() override
	{
		Sensor::ResetRegistry();
		realSensor.Register();
	}

	void TearDown() override
	{
		Sensor::ResetRegistry();
	}
};

TEST_F(SensorMocking, MockSensor)
{
	// Set a value on the "real" sensor
	realSensor.Set(25.0f);

	// And expect to see it
	{
		auto result = Sensor::Get(SensorType::Clt);
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 25.0f);
	}

	// Now set a mock value
	Sensor::SetMockValue(SensorType::Clt, 37.0f);

	// And expect to see that
	{
		auto result = Sensor::Get(SensorType::Clt);
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 37.0f);
	}
}

TEST_F(SensorMocking, ResetOne)
{
	// Set a value on the "real" sensor
	realSensor.Set(13.0f);

	// Now set a mock value
	Sensor::SetMockValue(SensorType::Clt, 21.0f);

	// Expect to see the mock value
	{
		auto result = Sensor::Get(SensorType::Clt);
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 21.0f);
	}

	// Reset that mock - it should go to the real value
	Sensor::ResetMockValue(SensorType::Clt);

	{
		auto result = Sensor::Get(SensorType::Clt);
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 13.0f);
	}
}

TEST_F(SensorMocking, ResetAll)
{
	// Set a value on the "real" sensor
	realSensor.Set(46.0f);

	// Now set a mock value
	Sensor::SetMockValue(SensorType::Clt, 33.0f);

	// Expect to see the mock value
	{
		auto result = Sensor::Get(SensorType::Clt);
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 33.0f);
	}

	// Reset all mocks - it should go to the real value
	Sensor::ResetAllMocks();

	{
		auto result = Sensor::Get(SensorType::Clt);
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 46.0f);
	}
}
