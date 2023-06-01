/**
 * @file test_one_cylinder_logic.cpp
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "spark_logic.h"

TEST(issues, issueOneCylinderSpecialCase968) {
	EngineTestHelper eth(engine_type_e::GY6_139QMB);
	setTable(config->injectionPhase, -180.0f);
	engineConfiguration->isFasterEngineSpinUpEnabled = false;
	engine->tdcMarkEnabled = false;
	// set injection_mode 1
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	setCrankOperationMode();

	eth.setTriggerType(trigger_type_e::TT_ONE);

	ASSERT_EQ( 0,  engine->executor.size()) << "start";

	eth.fireTriggerEvents2(/* count */ 2, 50 /* ms */);
	ASSERT_EQ( 0,  Sensor::getOrZero(SensorType::Rpm)) << "RPM";
	ASSERT_EQ( 0,  engine->executor.size()) << "first revolution(s)";

	eth.fireTriggerEvents2(/* count */ 1, 50 /* ms */);

	ASSERT_EQ( 2,  engine->executor.size()) << "first revolution(s)";
	eth.assertEvent5("spark up#0", 0, (void*)turnSparkPinHigh, -45167);
	eth.assertEvent5("spark down#0", 1, (void*)fireSparkAndPrepareNextSchedule, -39167);


	eth.fireTriggerEvents2(/* count */ 1, 50 /* ms */);
	ASSERT_EQ( 4,  engine->executor.size()) << "first revolution(s)";
}
