#include "pch.h"

#include "main_relay.h"

TEST(MainRelay, mr) {
	EngineTestHelper eth(TEST_ENGINE);

	MainRelayController dut;

	// Ignition is off, MR is off
	dut.onIgnitionStateChanged(false);
	dut.onSlowCallback();
	EXPECT_EQ(enginePins.mainRelay.getLogicValue(), false);

	// Ignition is now on, MR is on
	dut.onIgnitionStateChanged(true);
	dut.onSlowCallback();
	EXPECT_EQ(enginePins.mainRelay.getLogicValue(), true);
}
