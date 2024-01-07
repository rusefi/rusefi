/*
 * test_startOfCrankingPrimingPulse.cpp
 *
 *  Created on: Mar 4, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

TEST(engine, testPlainCrankingWithoutAdvancedFeatures) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setTable(config->injectionPhase, -180.0f);
	engineConfiguration->isFasterEngineSpinUpEnabled = false;
	engine->tdcMarkEnabled = false;
	engineConfiguration->cranking.baseFuel = 12;

	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);
	ASSERT_EQ( 0,  Sensor::getOrZero(SensorType::Rpm)) << "RPM=0";

	eth.fireTriggerEventsWithDuration(/* durationMs */ 200);
	// still no RPM since need to cycles measure cycle duration
	ASSERT_EQ( 0,  Sensor::getOrZero(SensorType::Rpm)) << "start-RPM#1";


	eth.fireRise(/* delayMs */ 200);
	ASSERT_EQ( 300,  Sensor::getOrZero(SensorType::Rpm)) << "RPM#2";
	// two simultaneous injections
	ASSERT_EQ( 4,  engine->executor.size()) << "plain#2";

	eth.assertEvent5("sim start", 0, (void*)startSimultaneousInjection, 100000 - 1625);
	// -1 because ugh floating point math
	eth.assertEvent5("sim end", 1, (void*)endSimultaneousInjection, 100000 - 1);
}


TEST(priming, startScheduling) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	ASSERT_EQ( 0,  Sensor::getOrZero(SensorType::Rpm)) << "RPM=0";

	// Turn on the ignition switch!
	engine->module<PrimeController>()->onIgnitionStateChanged(true);

	ASSERT_EQ(1, engine->executor.size()) << "prime fuel";
}

TEST(priming, duration) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	MockInjectorModel2 injectorModel;
	engine->module<InjectorModelPrimary>().set(&injectorModel);

	for (size_t i = 0; i < efi::size(engineConfiguration->primeBins); i++) {
		engineConfiguration->primeBins[i] = i * 10;
	}

	EXPECT_CALL(injectorModel, getInjectionDuration(0.075f)).WillOnce(Return(20.0f));
	engineConfiguration->primeValues[5] = 75;	// <-- We test this point

	// With coolant temp, we should see value from mock
	Sensor::setMockValue(SensorType::Clt, 50);
	EXPECT_EQ(20, engine->module<PrimeController>()->getPrimeDuration());

	// Without coolant temp, no prime
	Sensor::resetMockValue(SensorType::Clt);
	EXPECT_EQ(0, engine->module<PrimeController>()->getPrimeDuration());
}
