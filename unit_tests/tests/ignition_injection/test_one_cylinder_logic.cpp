/**
 * @file test_one_cylinder_logic.cpp
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "spark_logic.h"
#include "GY6_139QMB.h"
#include "defaults.h"
#include "fuel_math.h"

TEST(issues, issueOneCylinderSpecialCase968) {
	EngineTestHelper eth(engine_type_e::GY6_139QMB);
	angle_t timing = 4;
	engineConfiguration->crankingTimingAngle = timing;
	setTable(config->injectionPhase, -180.0f);
	engineConfiguration->isFasterEngineSpinUpEnabled = false;
	engine->tdcMarkEnabled = false;
	// set injection_mode 1
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	setCrankOperationMode();

	eth.setTriggerType(trigger_type_e::TT_HALF_MOON);

	ASSERT_EQ( 0,  engine->scheduler.size()) << "start";

	eth.fireTriggerEvents2(/* count */ 2, 50 /* ms */);
	ASSERT_EQ(0, Sensor::getOrZero(SensorType::Rpm));
	ASSERT_EQ( 0,  engine->scheduler.size()) << "first revolution(s)";

	eth.fireTriggerEvents2(/* count */ 1, 50 /* ms */);
	ASSERT_EQ(600, Sensor::getOrZero(SensorType::Rpm));
	ASSERT_EQ(engine->triggerCentral.currentEngineDecodedPhase, 90 + Gy6139_globalTriggerAngleOffset);
	ASSERT_EQ(engine->engineState.timingAdvance[0], timing);

  angle_t expectedAngle = 180 - Gy6139_globalTriggerAngleOffset + timing;
  int expectedDeltaTimeUs = eth.angleToTimeUs(expectedAngle);

	ASSERT_EQ( 2,  engine->scheduler.size()) << "first revolution(s)";

	auto const turnSparkPinHighStartChargingAction{ action_s::make<turnSparkPinHighStartCharging>((IgnitionEvent*){}) };
	auto const fireSparkAndPrepareNextScheduleAction{ action_s::make<fireSparkAndPrepareNextSchedule>((IgnitionEvent*){}) };

	eth.assertEvent5("spark up#0", 0, turnSparkPinHighStartChargingAction, -expectedDeltaTimeUs - MS2US(DEFAULT_CRANKING_DWELL_MS));
	eth.assertEvent5("spark down#0", 1, fireSparkAndPrepareNextScheduleAction, -expectedDeltaTimeUs);


	eth.fireTriggerEvents2(/* count */ 1, 50 /* ms */);
	ASSERT_EQ( 4,  engine->scheduler.size()) << "first revolution(s)";
}

/**
 * #8345: batch pairs each injector with the cylinder 360 degrees later in the firing order, so on
 * multi-cylinder engines every injector fires twice per cycle and half fuel per pulse is correct.
 * With one cylinder there is no distinct twin - InjectionEvent::update() computes secondIndex as
 * the same injector, and FuelSchedule only ever holds one event per cylinder - so only one pulse
 * is produced per cycle, yet the fuel is still halved, leaving the engine running lean.
 *
 * This test documents CURRENT behavior: one-cylinder batch still uses the multi-cylinder numbers.
 * Once #8345 is fixed, expect multiplier 1 and injection count 1 here.
 */
TEST(issues, oneCylinderBatchCurrentlyHalvesFuel) {
	EngineTestHelper eth(engine_type_e::GY6_139QMB);
	ASSERT_EQ(1, engineConfiguration->cylindersCount) << "GY6 is the one cylinder engine";

	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	float sequential = getInjectionModeDurationMultiplier();

	engineConfiguration->injectionMode = IM_BATCH;
	float batch = getInjectionModeDurationMultiplier();

	EXPECT_FLOAT_EQ(1, sequential);
	// only one pulse per cycle actually happens, so this halving is a known bug (#8345)
	EXPECT_FLOAT_EQ(0.5f, batch);

	// duty cycle estimate also assumes a second pulse which never happens (#8345)
	EXPECT_EQ(2, getNumberOfInjections(IM_BATCH));
}

TEST(issues, multiCylinderBatchHalvesFuel) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->injectionMode = IM_BATCH;

	// two cylinders: injector 0 fires on its own event and again on cylinder 1's event,
	// so two pulses per cycle and half the fuel each is correct
	engineConfiguration->cylindersCount = 2;
	EXPECT_FLOAT_EQ(0.5f, getInjectionModeDurationMultiplier());
	EXPECT_EQ(2, getNumberOfInjections(IM_BATCH));

	engineConfiguration->cylindersCount = 4;
	EXPECT_FLOAT_EQ(0.5f, getInjectionModeDurationMultiplier());
	EXPECT_EQ(2, getNumberOfInjections(IM_BATCH));
}
