/**
 * @file test_logic_expression.cpp
 *
 * https://sourceforge.net/p/rusefi/tickets/102/
 *
 * @date Oct 3, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

TEST(Actuators, FuelPump) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	FuelPumpController dut;

	// Mock a fuel pump pin
	engineConfiguration->fuelPumpPin = Gpio::A0;
	// Re-init so it picks up the new config
	enginePins.fuelPumpRelay.init();

	// ECU just started, haven't seen trigger yet
	setTimeNowUs(0.5e6);
	dut.onIgnitionStateChanged(true);
	dut.onSlowCallback();
	// Pump should be on!
	EXPECT_TRUE(efiReadPin(Gpio::A0));

	// Long time since ecu start, haven't seen trigger yet
	dut.onIgnitionStateChanged(true);
	advanceTimeUs(10e6);
	dut.onSlowCallback();
	// Pump should be off!
	EXPECT_FALSE(efiReadPin(Gpio::A0));

	// Long time since ecu start, just saw a trigger!
	dut.onIgnitionStateChanged(true);
	advanceTimeUs(10e6);
	engine->triggerCentral.handleShaftSignal(SHAFT_PRIMARY_FALLING, getTimeNowNt());
	dut.onSlowCallback();
	// Pump should be on!
	EXPECT_TRUE(efiReadPin(Gpio::A0));

	// ECU just started, and we just saw a trigger!
	dut.onIgnitionStateChanged(true);
	engine->triggerCentral.handleShaftSignal(SHAFT_PRIMARY_FALLING, getTimeNowNt());
	dut.onSlowCallback();
	// Pump should be on!
	EXPECT_TRUE(efiReadPin(Gpio::A0));
}
