#include "pch.h"

#include "stored_value_sensor.h"

class SensorBasic : public ::testing::Test {
protected:
	void SetUp() override {
		Sensor::resetRegistry();
	}

	void TearDown() override {
		Sensor::resetRegistry();
	}
};

TEST_F(SensorBasic, Registry) {
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

TEST_F(SensorBasic, DoubleRegister) {
	// Create a sensor, register it
	MockSensor dut(SensorType::Tps1);
	ASSERT_TRUE(dut.Register());

	// And then do it again!
	MockSensor dut2(SensorType::Tps1);
	EXPECT_FATAL_ERROR(dut2.Register());

	// Make sure that we get the first DUT back - not the second
	auto shouldBeDut = Sensor::getSensorOfType(SensorType::Tps1);
	EXPECT_EQ(shouldBeDut, &dut);
	EXPECT_NE(shouldBeDut, &dut2);
}

TEST_F(SensorBasic, SensorNotInitialized) {
	auto result = Sensor::get(SensorType::Clt);

	EXPECT_FALSE(result.Valid);
}

TEST_F(SensorBasic, SensorInitialized) {
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

TEST_F(SensorBasic, HasSensor) {
	MockSensor dut(SensorType::Clt);

	// Check that we don't have the sensor
	ASSERT_FALSE(Sensor::hasSensor(SensorType::Clt));

	// Register it
	ASSERT_TRUE(dut.Register());

	// Now we should!
	ASSERT_TRUE(Sensor::hasSensor(SensorType::Clt));

	// Check that we can have the sensor report that it's missing
	dut.setHasSensor(false);
	ASSERT_FALSE(Sensor::hasSensor(SensorType::Clt));
}

TEST_F(SensorBasic, HasSensorMock) {
	// Check that we don't have the sensor
	ASSERT_FALSE(Sensor::hasSensor(SensorType::Clt));

	// Mock the sensor - this should count as having it
	Sensor::setMockValue(SensorType::Clt, 25);

	// Now we should!
	ASSERT_TRUE(Sensor::hasSensor(SensorType::Clt));
}


TEST_F(SensorBasic, FindByName) {
	ASSERT_EQ(SensorType::Clt, findSensorTypeByName("Clt"));
}
