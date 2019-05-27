/*
 * test_fuelCut.cpp
 *
 *  Created on: Mar 22, 2018
 */

#include "engine_math.h"
#include "engine_test_helper.h"
#include "event_queue.h"
#include "tps.h"
#include "fsio_impl.h"

TEST(fuelCut, coasting) {
	printf("*************************************************** testCoastingFuelCut\r\n");

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	// configure coastingFuelCut
	engineConfiguration->bc.coastingFuelCutEnabled = true;
	engineConfiguration->coastingFuelCutRpmLow = 1300;
	engineConfiguration->coastingFuelCutRpmHigh = 1500;
	engineConfiguration->coastingFuelCutTps = 2;
	engineConfiguration->coastingFuelCutClt = 30;
	engineConfiguration->coastingFuelCutMap = 100;
	// set cranking threshold
	engineConfiguration->cranking.rpm = 999;
	// configure TPS
	engineConfiguration->tpsMin = 0;
	engineConfiguration->tpsMax = 10;

	// basic engine setup
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);

	// mock CLT - just above threshold ('hot engine')
	float hotClt = engine->sensors.clt = engineConfiguration->coastingFuelCutClt + 1;
	// mock TPS - throttle is opened
	setMockTpsAdc(6);
	// set 'running' RPM - just above RpmHigh threshold
	engine->rpmCalculator.mockRpm = engineConfiguration->coastingFuelCutRpmHigh + 1;
	// 'advance' time (amount doesn't matter)
	eth.moveTimeForwardUs(1000);

	const float normalInjDuration = 1.5f;
	/*
	 * We need to pass through all rpm changes (high-mid-low-mid-high) because of state-machine
	 */

	// process
	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

	// this is normal injection mode (the throttle is opened), no fuel cut-off
	assertEqualsM("inj dur#1 norm", normalInjDuration, ENGINE(injectionDuration));

	// 'releasing' the throttle
	setMockTpsAdc(0);
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


TEST(fuelCut, criticalEngineTemperature) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);

	engineConfiguration->useFSIO5ForCriticalIssueEngineStop = true;
	setFsio(MAGIC_OFFSET_FOR_CRITICAL_ENGINE, GPIOD_7, TOO_HOT_LOGIC PASS_CONFIG_PARAMETER_SUFFIX);
	applyFsioConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);

	// we need some non-zero time as getTimeNow() which would become stopEngineRequestTimeNt
	eth.moveTimeForwardUs(1000);

	engine->rpmCalculator.mockRpm = 2000;
	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	eth.engine.periodicSlowCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	ASSERT_EQ(engine->stopEngineRequestTimeNt, 0);

	ASSERT_FALSE(engine->stopEngineRequestTimeNt > 0);

	engine->sensors.mockClt = 200; // 200C is really hot!
	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	eth.engine.periodicSlowCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

	ASSERT_TRUE(engine->stopEngineRequestTimeNt > 0);
}
