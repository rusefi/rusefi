#include "pch.h"

TEST(engine, testRpmAcceleration) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_HALF_MOON);

	// first revolution
	eth.smartFireTriggerEvents2(/*count*/1, /*delay*/ 40);
    ASSERT_EQ(0u,  engine->triggerCentral.triggerState.totalTriggerErrorCounter);
	ASSERT_EQ(1500, Sensor::getOrZero(SensorType::Rpm));
	// no acceleration info YET, huh?
	ASSERT_EQ(0, engine->rpmCalculator.getRpmAcceleration());

	// second revolution same speed
	eth.smartFireTriggerEvents2(/*count*/1, /*delay*/ 40);
	ASSERT_EQ(0u,  engine->triggerCentral.triggerState.totalTriggerErrorCounter);
	ASSERT_EQ(1500, Sensor::getOrZero(SensorType::Rpm));
	// we got RPM we got acceleration
	ASSERT_EQ(9375, engine->rpmCalculator.getRpmAcceleration());

	// third revolution slow down
	eth.smartFireTriggerEvents2(/*count*/1, /*delay*/ 80);
	ASSERT_EQ(1000, Sensor::getOrZero(SensorType::Rpm));
	ASSERT_NEAR(-2083.3335, engine->rpmCalculator.getRpmAcceleration(), EPS3D);
    ASSERT_EQ(0u,  engine->triggerCentral.triggerState.totalTriggerErrorCounter);
}

// Coverage of a present defect, asserting current behavior: rpmRate is only computed in the
// once-per-engine-cycle RPM path, which "Always use instant RPM" skips, so the RPM acceleration
// channel never moves in that mode. FOME computes it from instant RPM: see FOME commits
// 04780d54b0 "fix rpmRate in case of always instant RPM" and e3978a2630.
TEST(engine, rpmAccelerationStaysZeroWithAlwaysInstantRpm) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->alwaysInstantRpm = true;
	eth.setTriggerType(trigger_type_e::TT_HALF_MOON);

	eth.smartFireTriggerEvents2(/*count*/3, /*delay*/ 40);
	ASSERT_EQ(0u, engine->triggerCentral.triggerState.totalTriggerErrorCounter);
	const float steadyRpm = Sensor::getOrZero(SensorType::Rpm);
	ASSERT_NEAR(1500, steadyRpm, 1);
	EXPECT_EQ(0, engine->rpmCalculator.getRpmAcceleration());

	// slow down: the same step reads about -2083 RPM/s without instant RPM, see testRpmAcceleration
	eth.smartFireTriggerEvents2(/*count*/2, /*delay*/ 80);
	ASSERT_EQ(0u, engine->triggerCentral.triggerState.totalTriggerErrorCounter);
	ASSERT_NEAR(750, Sensor::getOrZero(SensorType::Rpm), 1);
	// Expected after the fix: a negative rate
	EXPECT_EQ(0, engine->rpmCalculator.getRpmAcceleration());
}
