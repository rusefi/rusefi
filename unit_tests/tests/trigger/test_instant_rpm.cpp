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

	// perfectly uniform trigger events: no RPM variation within a cycle
	EXPECT_FLOAT_EQ(0, engine->triggerCentral.instantRpm.getInstantRpmRange())
		<< "steady RPM must produce zero instant RPM range";
}

TEST(instantRpm, rangePerCycle) {
	EngineTestHelper eth(engine_type_e::TEST_CRANK_ENGINE);
	engineConfiguration->twoStroke = true;

	engineConfiguration->trigger.customTotalToothCount = 4;
	engineConfiguration->trigger.customSkippedToothCount = 0;
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL);

	// settle at a steady speed: 4 teeth x 2 edges x 40ms = 320ms per revolution
	eth.smartFireTriggerEvents2(/*count*/200, /*delay*/ 40);
	EXPECT_FLOAT_EQ(0, engine->triggerCentral.instantRpm.getInstantRpmRange())
		<< "steady RPM must produce zero instant RPM range";
	float steadyRpm = engine->triggerCentral.instantRpm.getInstantRpm();

	// speed step: 20ms edges = double the RPM. Fire two full cycles so the
	// mixed-speed cycle completes and publishes its range.
	eth.smartFireTriggerEvents2(/*count*/8, /*delay*/ 20);
	float transitionRange = engine->triggerCentral.instantRpm.getInstantRpmRange();
	float fastRpm = engine->triggerCentral.instantRpm.getInstantRpm();
	EXPECT_GT(transitionRange, 0) << "RPM step within a cycle must produce non-zero range";
	// range cannot exceed the full speed step
	EXPECT_LE(transitionRange, fastRpm - steadyRpm + 1);

	// settle at the new speed: range returns to zero
	eth.smartFireTriggerEvents2(/*count*/200, /*delay*/ 20);
	EXPECT_FLOAT_EQ(0, engine->triggerCentral.instantRpm.getInstantRpmRange())
		<< "steady RPM at new speed must produce zero instant RPM range";
}
