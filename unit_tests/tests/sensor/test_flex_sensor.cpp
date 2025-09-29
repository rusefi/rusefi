#include "pch.h"
#include "flex_sensor.h"

TEST(FlexSensorTest, lowPinStart) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	FlexSensor sensor(MS2NT(1000));
	sensor.Register(true);

	// low to high transition after 0.5 sec
	sensor.callback(MS2NT(500), 1);

	ASSERT_EQ(0u, getRecentWarnings()->getCount());
}

TEST(FlexSensorTest, highPinStart) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	FlexSensor sensor(MS2NT(1000));
	sensor.Register(true);

	// low to high transition after 0.5 sec
	sensor.callback(MS2NT(500), 0);

	ASSERT_EQ(0u, getRecentWarnings()->getCount());
}

TEST(FlexSensorTest, converter) {
	efitick_t nowNt = 0;
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	FlexSensor sensor(MS2NT(1000));
	sensor.Register(true);

	// feed BiQuad filters and wait it get close to expected values
	for (size_t i = 0; i < 1000; i++) {
		sensor.callback(getTimeNowNt(), 1);
		// emulate 25 degrees C
		eth.moveTimeForwardUs(2576);
		sensor.callback(getTimeNowNt(), 0);
		// emulate 100 Hz = 50% ethanol
		eth.moveTimeForwardUs(10000 - 2576);
	}

	EXPECT_NEAR(50, Sensor::getOrZero(SensorType::FuelEthanolPercent), 0.1);
	EXPECT_NEAR(25, Sensor::getOrZero(SensorType::FuelTemperature), 0.1);
}
