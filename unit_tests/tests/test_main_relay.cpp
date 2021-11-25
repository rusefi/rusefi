#include "pch.h"

#include "main_relay.h"

TEST(MainRelay, mr) {
	EngineTestHelper eth(TEST_ENGINE);

	MainRelayController dut;

	// Low battery voltage, MR is off
	Sensor::setMockValue(SensorType::BatteryVoltage, 1);
	dut.onSlowCallback();
	EXPECT_EQ(enginePins.mainRelay.getLogicValue(), false);

	// Ignition is now on, MR is on
	Sensor::setMockValue(SensorType::BatteryVoltage, 12);
	dut.onSlowCallback();
	EXPECT_EQ(enginePins.mainRelay.getLogicValue(), true);
}
