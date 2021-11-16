/*
 * @file test_fuelCut.cpp
 *
 * @date Mar 22, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "event_queue.h"
#include "fsio_impl.h"

using ::testing::_;

TEST(fuelCut, coasting) {
	EngineTestHelper eth(TEST_ENGINE);
	EXPECT_CALL(eth.mockAirmass, getAirmass(_))
		.WillRepeatedly(Return(AirmassResult{0.1008f, 50.0f}));

	// configure coastingFuelCut
	engineConfiguration->coastingFuelCutEnabled = true;
	engineConfiguration->coastingFuelCutRpmLow = 1300;
	engineConfiguration->coastingFuelCutRpmHigh = 1500;
	engineConfiguration->coastingFuelCutTps = 2;
	engineConfiguration->coastingFuelCutClt = 30;
	engineConfiguration->coastingFuelCutMap = 100;
	// set cranking threshold
	engineConfiguration->cranking.rpm = 999;

	// basic engine setup
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);

	Sensor::setMockValue(SensorType::Map, 0);

	// mock CLT - just above threshold ('hot engine')
	float hotClt = engineConfiguration->coastingFuelCutClt + 1;
	Sensor::setMockValue(SensorType::Clt, hotClt);
	// mock TPS - throttle is opened
	Sensor::setMockValue(SensorType::Tps1, 60);
	// set 'running' RPM - just above RpmHigh threshold
	engine->rpmCalculator.mockRpm = engineConfiguration->coastingFuelCutRpmHigh + 1;
	// 'advance' time (amount doesn't matter)
	eth.moveTimeForwardUs(1000);

	const float normalInjDuration = 1.5f;
	/*
	 * We need to pass through all rpm changes (high-mid-low-mid-high) because of state-machine
	 */

	// process
	eth.engine.periodicFastCallback();

	// this is normal injection mode (the throttle is opened), no fuel cut-off
	assertEqualsM("inj dur#1 norm", normalInjDuration, ENGINE(injectionDuration));

	// 'releasing' the throttle
	Sensor::setMockValue(SensorType::Tps1, 0);
	eth.engine.periodicFastCallback();

	// Fuel cut-off is enabled now
	assertEqualsM("inj dur#2 cut", 0.0f, ENGINE(injectionDuration));

	// Now drop the CLT below threshold
	Sensor::setMockValue(SensorType::Clt, engineConfiguration->coastingFuelCutClt - 1);
	eth.engine.periodicFastCallback();

	// Fuel cut-off should be diactivated - the engine is 'cold'
	assertEqualsM("inj dur#3 clt", normalInjDuration, ENGINE(injectionDuration));

	// restore CLT
	Sensor::setMockValue(SensorType::Clt, hotClt);
	// And set RPM - somewhere between RpmHigh and RpmLow threshold
	engine->rpmCalculator.mockRpm = (engineConfiguration->coastingFuelCutRpmHigh + engineConfiguration->coastingFuelCutRpmLow) / 2;
	eth.engine.periodicFastCallback();

	// Fuel cut-off is enabled - nothing should change
	assertEqualsM("inj dur#4 mid", normalInjDuration, ENGINE(injectionDuration));

	// Now drop RPM just below RpmLow threshold
	engine->rpmCalculator.mockRpm = engineConfiguration->coastingFuelCutRpmLow - 1;
	eth.engine.periodicFastCallback();

	// Fuel cut-off is now disabled (the engine is idling)
	assertEqualsM("inj dur#5 idle", normalInjDuration, ENGINE(injectionDuration));

	// Now change RPM just below RpmHigh threshold
	engine->rpmCalculator.mockRpm = engineConfiguration->coastingFuelCutRpmHigh - 1;
	eth.engine.periodicFastCallback();

	// Fuel cut-off is still disabled
	assertEqualsM("inj dur#6 mid", normalInjDuration, ENGINE(injectionDuration));

	// Now set RPM just above RpmHigh threshold
	engine->rpmCalculator.mockRpm = engineConfiguration->coastingFuelCutRpmHigh + 1;
	eth.engine.periodicFastCallback();

	// Fuel cut-off is active again!
	assertEqualsM("inj dur#7 cut", 0.0f, ENGINE(injectionDuration));
}

