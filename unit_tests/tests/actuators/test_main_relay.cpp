#include "pch.h"

#include "main_relay.h"

TEST(Actuators, MainRelay) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	MainRelayController dut;

	// Ignition is off, MR is off
	dut.onSlowCallback();
	EXPECT_EQ(enginePins.mainRelay.getLogicValue(), false);

	// Battery above threshold - MR is on
	Sensor::setMockValue(SensorType::BatteryVoltage, 13);
	dut.onSlowCallback();
	EXPECT_EQ(enginePins.mainRelay.getLogicValue(), true);
}
