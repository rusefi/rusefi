#include "pch.h"
#include "spark_logic.h"
#include "harley.h"

TEST(OddFire, hd) {
	EngineTestHelper eth(engine_type_e::HARLEY);
	engineConfiguration->cranking.rpm = 100;
	engineConfiguration->vvtMode[0] = VVT_SINGLE_TOOTH; // need to avoid engine phase sync requirement
	float cylinderOne = 19;
	float cylinderTwo = 13;
	engineConfiguration->timing_offset_cylinder[0] = cylinderOne;
	engineConfiguration->timing_offset_cylinder[1] = -cylinderTwo;
	angle_t timing = 1;
	setTable(config->ignitionTable, timing);

	engineConfiguration->crankingTimingAngle = timing;
	engine->tdcMarkEnabled = false;
	engineConfiguration->camInputs[0] = Gpio::Unassigned;
	eth.setTriggerType(trigger_type_e::TT_HALF_MOON);

	eth.fireTriggerEvents2(2 /* count */ , 60 /* ms */);
	LimpState limitedSparkState = getLimpManager()->allowIgnition();
	ASSERT_TRUE(limitedSparkState.value);
	ASSERT_EQ(limitedSparkState.reason, ClearReason::None);

//	ASSERT_NEAR(-220.0, eth.timeToAngle(-73.333333), EPS3D);
//	ASSERT_NEAR(-200.0, eth.timeToAngle(-66.66666), EPS3D);
//	ASSERT_NEAR(160.0, eth.timeToAngle(53.333333), EPS3D);
//	ASSERT_NEAR(-168.0, eth.timeToAngle(-56.0), EPS3D);

	float expectedAngle3 = -180 - cylinderOne - timing;

	ASSERT_EQ( 8,  engine->executor.size());
	eth.assertEvent5("spark down#3", 3, (void*)fireSparkAndPrepareNextSchedule, eth.angleToTimeUs(expectedAngle3));

	float expectedAngle7 = -180 - cylinderOne - timing;
	eth.assertEvent5("spark down#7", 3, (void*)fireSparkAndPrepareNextSchedule, eth.angleToTimeUs(expectedAngle7));


	eth.assertRpm( 500, "spinning-RPM#1");

	engine->executor.executeAll(eth.getTimeNowUs() + MS2US(1000000));


	expectedAngle3 = 180 - cylinderOne - timing;
	eth.fireTriggerEvents2(2 /* count */ , 60 /* ms */);
	ASSERT_EQ( 4,  engine->executor.size());
	eth.assertEvent5("spark down2#3", 3, (void*)fireSparkAndPrepareNextSchedule, eth.angleToTimeUs(expectedAngle3));
}
