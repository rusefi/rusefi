#include "pch.h"

#include "sensor_reader.h"

class SensorBasicReader : public ::testing::Test {
protected:
	void SetUp() override {
		Sensor::resetRegistry();
	}

	void TearDown() override {
		Sensor::resetRegistry();
	}
};

TEST_F(SensorBasicReader, Value) {
	// Create a sensor - but don't register it
	MockSensor dut(SensorType::Tps1);

	// Create a reader for the same sensor type - fallback value = 123
	SensorReader<SensorType::Tps1> dutReader(123.0f);

	// Expect not to find it
	{
		auto result = dutReader.get();
		EXPECT_FALSE(result.Valid);
	}

	// Register the sensor now
	EXPECT_TRUE(dut.Register());

	// Still expect invalid - no value has been set yet
	{
		auto result = dutReader.get();
		EXPECT_FALSE(result.Valid);
	}

	dut.set(47.0f);

	// Expect valid - with the value 47 we just set
	{
		auto result = dutReader.get();
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 47.0f);
	}
}

TEST_F(SensorBasicReader, FallbackValue) {
	// Create a sensor - but don't register it
	MockSensor dut(SensorType::Tps1);

	// Create a reader for the same sensor type - fallback value = 123
	SensorReader<SensorType::Tps1> dutReader(123.0f);

	// Expect not to find it
	{
		auto result = dutReader.get();
		EXPECT_FALSE(result.Valid);
	}

	// Register the sensor now
	EXPECT_TRUE(dut.Register());

	// Still expect invalid - no value has been set yet
	{
		auto result = dutReader.get();
		EXPECT_FALSE(result.Valid);
	}

	// However - we should be able to get the fallback value
	{
		auto result = dutReader.getOrDefault();
		EXPECT_FLOAT_EQ(result, 123.0f);
	}

	// Now set the value for real, and ensure we get that instead
	dut.set(63);

	{
		auto result = dutReader.getOrDefault();
		EXPECT_FLOAT_EQ(result, 63.0f);
	}
}
