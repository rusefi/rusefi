#include "pch.h"
#include "flex_sensor.h"

TEST(FlexSensorTest, lowPinStart) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	FlexSensor sensor(MS2NT(1000));
	sensor.Register(true);

	// low to high transition after 1 sec
	sensor.callback(MS2NT(500), 1);

	ASSERT_EQ(0u,  getRecentWarnings()->getCount());
}

TEST(FlexSensorTest, highPinStart) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	FlexSensor sensor(MS2NT(1000));
	sensor.Register(true);

	// low to high transition after 1 sec
	sensor.callback(MS2NT(500), 0);

	// here we get 'unit_test_warning: C6004: flex high 42949672.000000' warning
	ASSERT_EQ(1u,  getRecentWarnings()->getCount());
}
