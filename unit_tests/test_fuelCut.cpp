/*
 * test_fuelCut.cpp
 *
 *  Created on: Mar 22, 2018
 */

#include "engine_math.h"
#include "test_fuelCut.h"
#include "test_trigger_decoder.h"
#include "event_queue.h"
#include "unit_test_framework.h"
#include "tps.h"

extern EventQueue schedulingQueue;
extern int timeNowUs;
extern EnginePins enginePins;

void testCoastingFuelCut() {
	// this is just a reference unit test implementation
	printf("*************************************************** testCoastingFuelCut\r\n");

	EngineTestHelper eth(TEST_ENGINE);
	EXPAND_EngineTestHelper

	// configure coastingFuelCut
	engineConfiguration->bc.coastingFuelCutEnabled = true;
	engineConfiguration->coastingFuelCutRpmLow = 1300;
	engineConfiguration->coastingFuelCutRpmHigh = 1500;
	engineConfiguration->coastingFuelCutTps = 2;
	engineConfiguration->coastingFuelCutClt = 30;
	// set cranking threshold
	engineConfiguration->cranking.rpm = 999;
	// configure TPS
	eth.engine.engineConfiguration->tpsMin = 0;
	eth.engine.engineConfiguration->tpsMax = 10;

	// basic engine setup
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);

	// mock CLT - just above threshold ('hot engine')
	float hotClt = engine->sensors.clt = engineConfiguration->coastingFuelCutClt + 1;
	// mock TPS - throttle is opened
	setMockTpsPosition(6);
	// set 'running' RPM - just above RpmHigh threshold
	engine->rpmCalculator.mockRpm = engineConfiguration->coastingFuelCutRpmHigh + 1;
	// 'advance' time (amount doesn't matter)
	timeNowUs += 1000;

	const float normalInjDuration = 1.5f;
	/*
	 * We need to pass through all rpm changes (high-mid-low-mid-high) because of state-machine
	 */

	// process
	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

	// this is normal injection mode (the throttle is opened), no fuel cut-off
	assertEqualsM("inj dur#1 norm", normalInjDuration, ENGINE(injectionDuration));

	// 'releasing' the throttle
	setMockTpsPosition(0);
	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

	// Fuel cut-off is enabled now
	assertEqualsM("inj dur#2 cut", 0.0f, ENGINE(injectionDuration));

	// Now drop the CLT below threshold
	engine->sensors.clt = engineConfiguration->coastingFuelCutClt - 1;
	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

	// Fuel cut-off should be diactivated - the engine is 'cold'
	assertEqualsM("inj dur#3 clt", normalInjDuration, ENGINE(injectionDuration));

	// restore CLT
	engine->sensors.clt = hotClt;
	// And set RPM - somewhere between RpmHigh and RpmLow threshold
	engine->rpmCalculator.mockRpm = (engineConfiguration->coastingFuelCutRpmHigh + engineConfiguration->coastingFuelCutRpmLow) / 2;
	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

	// Fuel cut-off is enabled - nothing should change
	assertEqualsM("inj dur#4 mid", normalInjDuration, ENGINE(injectionDuration));

	// Now drop RPM just below RpmLow threshold
	engine->rpmCalculator.mockRpm = engineConfiguration->coastingFuelCutRpmLow - 1;
	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

	// Fuel cut-off is now disabled (the engine is idling)
	assertEqualsM("inj dur#5 idle", normalInjDuration, ENGINE(injectionDuration));

	// Now change RPM just below RpmHigh threshold
	engine->rpmCalculator.mockRpm = engineConfiguration->coastingFuelCutRpmHigh - 1;
	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

	// Fuel cut-off is still disabled
	assertEqualsM("inj dur#6 mid", normalInjDuration, ENGINE(injectionDuration));

	// Now set RPM just above RpmHigh threshold
	engine->rpmCalculator.mockRpm = engineConfiguration->coastingFuelCutRpmHigh + 1;
	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

	// Fuel cut-off is active again!
	assertEqualsM("inj dur#7 cut", 0.0f, ENGINE(injectionDuration));
}

void testFuelCut() {
	testCoastingFuelCut();
}
