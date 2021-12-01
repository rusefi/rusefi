#include "pch.h"

class SensorMocking : public ::testing::Test {
protected:
	MockSensor realSensor;

	SensorMocking()
		: realSensor(SensorType::Clt) {}

	void SetUp() override {
		Sensor::resetRegistry();
		ASSERT_TRUE(realSensor.Register());
	}

	void TearDown() override {
		Sensor::resetRegistry();
	}
};

TEST_F(SensorMocking, MockSensor) {
	// Set a value on the "real" sensor
	realSensor.set(25.0f);

	// And expect to see it
	{
		auto result = Sensor::get(SensorType::Clt);
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 25.0f);
	}

	// Now set a mock value
	Sensor::setMockValue(SensorType::Clt, 37.0f);

	// And expect to see that
	{
		auto result = Sensor::get(SensorType::Clt);
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 37.0f);
	}
}

TEST_F(SensorMocking, ResetOne) {
	// Set a value on the "real" sensor
	realSensor.set(13.0f);

	// Now set a mock value
	Sensor::setMockValue(SensorType::Clt, 21.0f);

	// Expect to see the mock value
	{
		auto result = Sensor::get(SensorType::Clt);
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 21.0f);
	}

	// Reset that mock - it should go to the real value
	Sensor::resetMockValue(SensorType::Clt);

	{
		auto result = Sensor::get(SensorType::Clt);
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 13.0f);
	}
}

TEST_F(SensorMocking, ResetAll) {
	// Set a value on the "real" sensor
	realSensor.set(46.0f);

	// Now set a mock value
	Sensor::setMockValue(SensorType::Clt, 33.0f);

	// Expect to see the mock value
	{
		auto result = Sensor::get(SensorType::Clt);
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 33.0f);
	}

	// Reset all mocks - it should go to the real value
	Sensor::resetAllMocks();

	{
		auto result = Sensor::get(SensorType::Clt);
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 46.0f);
	}
}

TEST_F(SensorMocking, ResetWithResetRegistry) {
	// Set a value on the "real" sensor
	realSensor.set(46.0f);

	// Now set a mock value
	Sensor::setMockValue(SensorType::Clt, 33.0f);

	// Expect to see the mock value
	{
		auto result = Sensor::get(SensorType::Clt);
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 33.0f);
	}

	// Reset registry - including mocks
	Sensor::resetRegistry();

	{
		auto result = Sensor::get(SensorType::Clt);
		EXPECT_FALSE(result.Valid);
	}
}

