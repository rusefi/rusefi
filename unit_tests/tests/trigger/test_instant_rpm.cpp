/*
 * @file test_instant_rpm.cpp
 *
 * @date: feb 15, 2026
 * @author FDSoftware
 */

#include "pch.h"

TEST(instantRpm, singleToothTrigger) {
	EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
	engineConfiguration->twoStroke = true;

	engineConfiguration->trigger.customTotalToothCount = 1;
	engineConfiguration->trigger.customSkippedToothCount = 0;
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL);

	// spin up enough to get RPM sync
	eth.smartFireTriggerEvents2(/*count*/20, /*delay*/ 40);
	ASSERT_EQ(750, Sensor::getOrZero(SensorType::Rpm));

	// run more revolutions so instant RPM has time to settle
	eth.smartFireTriggerEvents2(/*count*/200, /*delay*/ 40);

	float instantRpm = engine->triggerCentral.instantRpm.getInstantRpm();

	// instantRpm should be close to the regular RPM (~750)
	EXPECT_NEAR(750, instantRpm, 1) << "instantRpm should match RPM with a single-tooth trigger";
}
