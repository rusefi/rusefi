#include "pch.h"

#include "main_relay.h"

TEST(MainRelay, Basic) {
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

extern WarningCodeState unitTestWarningCodeState;

TEST(MainRelay, VoltageSenseRelayOk) {
	setTimeNowUs(0);

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	MainRelayController dut;

	// Ignition is off, MR is off
	dut.onSlowCallback();
	EXPECT_EQ(enginePins.mainRelay.getLogicValue(), false);

	// Battery above threshold - MR turns on
	Sensor::setMockValue(SensorType::BatteryVoltage, 13);
	Sensor::setMockValue(SensorType::MainRelayVoltage, 0);
	dut.onSlowCallback();
	EXPECT_EQ(enginePins.mainRelay.getLogicValue(), true);

	// At 0.4 second, the main relay voltage comes up
	setTimeNowUs(0.4e6);
	Sensor::setMockValue(SensorType::MainRelayVoltage, 13);
	dut.onSlowCallback();
	EXPECT_EQ(0, unitTestWarningCodeState.recentWarnings.getCount());

	// A long time later, things should still be OK
	setTimeNowUs(10e6);
	dut.onSlowCallback();
	EXPECT_EQ(0, unitTestWarningCodeState.recentWarnings.getCount());
}

TEST(MainRelay, VoltageSenseRelayBroken) {
	setTimeNowUs(0);

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	MainRelayController dut;

	// Ignition is off, MR is off
	dut.onSlowCallback();
	EXPECT_EQ(enginePins.mainRelay.getLogicValue(), false);

	// Battery above threshold - MR turns on
	Sensor::setMockValue(SensorType::BatteryVoltage, 13);
	Sensor::setMockValue(SensorType::MainRelayVoltage, 0);
	dut.onSlowCallback();
	EXPECT_EQ(enginePins.mainRelay.getLogicValue(), true);

	// At 0.4 second, the main relay voltage comes up, but not very far
	setTimeNowUs(0.4e6);
	Sensor::setMockValue(SensorType::MainRelayVoltage, 8);
	dut.onSlowCallback();
	// at 0.4s we don't warn yet
	EXPECT_EQ(0, unitTestWarningCodeState.recentWarnings.getCount());

	// After 0.6s, this voltage is no longer acceptable and a warning should register
	setTimeNowUs(0.6e6);
	dut.onSlowCallback();
	EXPECT_EQ(1, unitTestWarningCodeState.recentWarnings.getCount());
}
