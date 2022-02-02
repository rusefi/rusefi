#include "pch.h"

#include "main_relay.h"

TEST(MainRelay, mainRelayLogic) {
	EngineTestHelper eth(TEST_ENGINE);

	MainRelayController dut;

	// Ignition is off, MR is off
	dut.onSlowCallback();
	EXPECT_EQ(enginePins.mainRelay.getLogicValue(), false);

	// Battery above threshold - MR is on
	Sensor::setMockValue(SensorType::BatteryVoltage, 13);
	dut.onSlowCallback();
	EXPECT_EQ(enginePins.mainRelay.getLogicValue(), true);
}
