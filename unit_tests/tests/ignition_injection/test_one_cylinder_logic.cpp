/**
 * @file test_one_cylinder_logic.cpp
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "spark_logic.h"
#include "GY6_139QMB.h"
#include "defaults.h"

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

	ASSERT_EQ( 0,  engine->executor.size()) << "start";

	eth.fireTriggerEvents2(/* count */ 2, 50 /* ms */);
	eth.assertRpm(0);
	ASSERT_EQ( 0,  engine->executor.size()) << "first revolution(s)";

	eth.fireTriggerEvents2(/* count */ 1, 50 /* ms */);
	eth.assertRpm(600, "RPM");
	ASSERT_EQ(engine->triggerCentral.currentEngineDecodedPhase, 90 + Gy6139_globalTriggerAngleOffset);
	ASSERT_EQ(engine->engineState.timingAdvance[0], timing);

  angle_t expectedAngle = 180 - Gy6139_globalTriggerAngleOffset + timing;
  int expectedDeltaTimeUs = eth.angleToTimeUs(expectedAngle);

	ASSERT_EQ( 2,  engine->executor.size()) << "first revolution(s)";
	eth.assertEvent5("spark up#0", 0, (void*)turnSparkPinHighStartCharging, -expectedDeltaTimeUs - MS2US(DEFAULT_CRANKING_DWELL_MS));
	eth.assertEvent5("spark down#0", 1, (void*)fireSparkAndPrepareNextSchedule, -expectedDeltaTimeUs);


	eth.fireTriggerEvents2(/* count */ 1, 50 /* ms */);
	ASSERT_EQ( 4,  engine->executor.size()) << "first revolution(s)";
}
