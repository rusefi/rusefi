/*
 * test_startOfCrankingPrimingPulse.cpp
 *
 *  Created on: Mar 4, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "injector_model.h"
#include "limp_manager.h"
#include "util/injection_crank_helper.h"

TEST(engine, testPlainCrankingWithoutAdvancedFeatures) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setTable(config->injectionPhase, -180.0f);
	engineConfiguration->isFasterEngineSpinUpEnabled = false;
	engine->tdcMarkEnabled = false;
	setTestFuelCrankingTable(12);

	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);
	ASSERT_EQ( 0,  Sensor::getOrZero(SensorType::Rpm)) << "RPM=0";

	eth.fireTriggerEventsWithDuration(/* durationMs */ 200);
	// still no RPM since need to cycles measure cycle duration
	ASSERT_EQ( 0,  Sensor::getOrZero(SensorType::Rpm)) << "start-RPM#1";


	eth.fireRise(/* delayMs */ 200);
	ASSERT_EQ(300, Sensor::getOrZero(SensorType::Rpm));
	// two simultaneous injections
	ASSERT_EQ( 4,  engine->scheduler.size()) << "plain#2";

	auto const startSimultaneousInjectionAction{ action_s::make<startInjection>(scheduler_arg_t{}) };
	auto const endSimultaneousInjectionAction{ action_s::make<endInjection>(scheduler_arg_t{})};

	eth.assertEvent5("sim start", 0, startSimultaneousInjectionAction, 100000 - 1625);
	// -1 because ugh floating point math
	eth.assertEvent5("sim end", 1, endSimultaneousInjectionAction, 100000 - 1);
}


TEST(priming, startScheduling) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	ASSERT_EQ( 0,  Sensor::getOrZero(SensorType::Rpm)) << "RPM=0";

	// Turn on the ignition switch!
	engine->module<PrimeController>()->onIgnitionStateChanged(true);

	ASSERT_EQ(1, engine->scheduler.size()) << "prime fuel";
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

// With flexCranking enabled and a flex sensor present, the priming mass comes from the 2D
// coolant-vs-ethanol table (primeFlexTable) instead of the 1D primeValues curve.
TEST(priming, flexTableDuration) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	MockInjectorModel2 injectorModel;
	engine->module<InjectorModelPrimary>().set(&injectorModel);

	// Linear coolant axis; the table is flat across coolant so only the ethanol axis matters.
	for (size_t i = 0; i < efi::size(engineConfiguration->primeBins); i++) {
		engineConfiguration->primeBins[i] = i * 10;
	}

	const uint8_t ethanolBins[] = { 0, 35, 65, 100 };
	copyArray(engineConfiguration->primeFlexBins, ethanolBins);

	const uint16_t rowMassMg[] = { 100, 200, 300, 400 };	// mg per ethanol row (multiples of the 5 mg LSB)
	for (size_t row = 0; row < efi::size(engineConfiguration->primeFlexTable); row++) {
		for (size_t col = 0; col < PRIME_CURVE_COUNT; col++) {
			engineConfiguration->primeFlexTable[row][col] = rowMassMg[row];
		}
	}

	engineConfiguration->flexCranking = true;
	Sensor::setMockValue(SensorType::Clt, 50);

	// E35 -> row 1 -> 200 mg -> 0.2 g
	EXPECT_CALL(injectorModel, getInjectionDuration(::testing::FloatNear(0.2f, 1e-4f))).WillOnce(Return(22.0f));
	Sensor::setMockValue(SensorType::FuelEthanolPercent, 35);
	EXPECT_EQ(22, engine->module<PrimeController>()->getPrimeDuration());

	// E50 -> halfway between rows at 35 (200 mg) and 65 (300 mg) -> 250 mg -> 0.25 g
	EXPECT_CALL(injectorModel, getInjectionDuration(::testing::FloatNear(0.25f, 1e-4f))).WillOnce(Return(28.0f));
	Sensor::setMockValue(SensorType::FuelEthanolPercent, 50);
	EXPECT_EQ(28, engine->module<PrimeController>()->getPrimeDuration());
}

namespace {

void configurePrimeForGateReproduction() {
	engineConfiguration->cylindersCount = 4;
	engineConfiguration->injectorFlowAsMassFlow = true;
	engineConfiguration->injector.flow = 1;
	engineConfiguration->injectorCompensationMode = ICM_None;
	engineConfiguration->injectorNonlinearMode = INJ_None;
	engineConfiguration->flexCranking = false;
	setTable(engineConfiguration->injector.battLagCorrTable, 0.0f);
	setArrayValues(engineConfiguration->primeValues, 10);
	setTable(config->lambdaTable, 1.0f);
	engineConfiguration->primingDelay = 0.5f;
	Sensor::setMockValue(SensorType::Clt, 20);
	Sensor::setMockValue(SensorType::Tps1, 0);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0);
	engine->module<InjectorModelPrimary>().unmock().prepare();
}

void expectPrimePulseAndDrain(EngineTestHelper& eth) {
	auto& prime = engine->module<PrimeController>().unmock();
	EXPECT_TRUE(prime.isPriming());
	for (size_t i = 0; i < engineConfiguration->cylindersCount; i++) {
		EXPECT_TRUE(enginePins.injectors[i].getLogicValue());
	}
	eth.moveTimeForwardAndInvokeEventsUs(11000);
	EXPECT_FALSE(prime.isPriming());
	for (size_t i = 0; i < engineConfiguration->cylindersCount; i++) {
		EXPECT_FALSE(enginePins.injectors[i].getLogicValue());
	}
}

void expectNoPrimePulse() {
	EXPECT_FALSE(engine->module<PrimeController>().unmock().isPriming());
	for (size_t i = 0; i < engineConfiguration->cylindersCount; i++) {
		EXPECT_FALSE(enginePins.injectors[i].getLogicValue());
	}
}

} // namespace

TEST(priming, PrimeRespectsDisabledInjection) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configurePrimeForGateReproduction();
	engineConfiguration->isInjectionEnabled = false;
	getLimpManager()->updateState(0, getTimeNowNt());
	ASSERT_EQ(ClearReason::Settings, getLimpManager()->allowInjection().reason);
	engine->module<PrimeController>()->onIgnitionStateChanged(true);
	eth.moveTimeForwardAndInvokeEventsUs(600000);
	expectNoPrimePulse();
}

TEST(priming, DelayedPrimeSkipsAfterEngineStarts) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configurePrimeForGateReproduction();
	engine->module<PrimeController>()->onIgnitionStateChanged(true);
	eth.moveTimeForwardAndInvokeEventsUs(100000);
	engine->rpmCalculator.setRpmValue(1000);
	ASSERT_TRUE(engine->rpmCalculator.isRunning());
	eth.moveTimeForwardAndInvokeEventsUs(500000);
	EXPECT_TRUE(engine->rpmCalculator.isRunning());
	expectNoPrimePulse();
}

TEST(priming, DelayedPrimeRespectsFloodClear) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configurePrimeForGateReproduction();
	engineConfiguration->isCylinderCleanupEnabled = true;
	engine->module<PrimeController>()->onIgnitionStateChanged(true);
	eth.moveTimeForwardAndInvokeEventsUs(100000);
	Sensor::setMockValue(SensorType::Tps1, 100);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 100);
	getLimpManager()->updateState(0, getTimeNowNt());
	ASSERT_EQ(ClearReason::FloodClear, getLimpManager()->allowInjection().reason);
	eth.moveTimeForwardAndInvokeEventsUs(500000);
	EXPECT_EQ(ClearReason::FloodClear, getLimpManager()->allowInjection().reason);
	expectNoPrimePulse();
}

TEST(priming, AllowedPrimeStillInjects) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configurePrimeForGateReproduction();
	getLimpManager()->updateState(0, getTimeNowNt());
	ASSERT_TRUE(getLimpManager()->allowInjection());
	engine->module<PrimeController>()->onIgnitionStateChanged(true);
	eth.moveTimeForwardAndInvokeEventsUs(600000);
	expectPrimePulseAndDrain(eth);
}
