#include "pch.h"
#include "spark_logic.h"
#include "harley.h"
#include "fuel_math.h"
#include "defaults.h"

using ::testing::_;

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

	EXPECT_CALL(*eth.mockAirmass, getAirmass(/*any rpm*/_, _))
		.WillRepeatedly(Return(AirmassResult{0.1008f, 50.0f}));

	engineConfiguration->crankingTimingAngle = timing;
	engine->tdcMarkEnabled = false;
	engineConfiguration->camInputs[0] = Gpio::Unassigned;
	eth.setTriggerType(trigger_type_e::TT_HALF_MOON);

	eth.fireTriggerEvents2(2 /* count */ , 60 /* ms */);
	LimpState limitedSparkState = getLimpManager()->allowIgnition();
	ASSERT_TRUE(limitedSparkState.value);
	ASSERT_EQ(limitedSparkState.reason, ClearReason::None);
	ASSERT_EQ(IM_SIMULTANEOUS, getCurrentInjectionMode()); // still spinning up
	ASSERT_NEAR(0.027, getInjectionMass(200), EPS3D);

//	ASSERT_NEAR(-220.0, eth.timeToAngle(-73.333333), EPS3D);
//	ASSERT_NEAR(140, eth.timeToAngle(46.66666), EPS3D);
//	ASSERT_NEAR(-200.0, eth.timeToAngle(-66.66666), EPS3D);
//	ASSERT_NEAR(160.0, eth.timeToAngle(53.333333), EPS3D);
//	ASSERT_NEAR(-168.0, eth.timeToAngle(-56.0), EPS3D);
//	ASSERT_NEAR(32, eth.timeToAngle(10.66666666), EPS3D);

	angle_t expectedAngle3 = -180 + cylinderTwo - timing;

	ASSERT_EQ( 8,  engine->executor.size());
	eth.assertEvent5("spark down#3", 3, (void*)fireSparkAndPrepareNextSchedule, eth.angleToTimeUs(expectedAngle3));

	angle_t expectedAngle7 = 180 - cylinderOne - timing;
	eth.assertEvent5("spark down#7", 7, (void*)fireSparkAndPrepareNextSchedule, eth.angleToTimeUs(expectedAngle7));

	eth.assertRpm( 500, "spinning-RPM#1");

	engine->executor.executeAll(eth.getTimeNowUs() + MS2US(1000000));

	eth.fireTriggerEvents2(2 /* count */ , 60 /* ms */);
	ASSERT_EQ(IM_SEQUENTIAL, getCurrentInjectionMode());
	ASSERT_NEAR(0.0069257142022, getInjectionMass(200), EPS3D);

	ASSERT_EQ( 8,  engine->executor.size());
	eth.assertEvent5("fuel down2#1", 1, (void*)turnInjectionPinLow, eth.angleToTimeUs(180 + PORT_INJECTION_OFFSET));
	eth.assertEvent5("spark down2#3", 3, (void*)fireSparkAndPrepareNextSchedule, eth.angleToTimeUs(-180 + cylinderTwo - timing));
	eth.assertEvent5("fuel down2#5", 5, (void*)turnInjectionPinLow, eth.angleToTimeUs(540 + PORT_INJECTION_OFFSET));
	eth.assertEvent5("spark down2#7", 7, (void*)fireSparkAndPrepareNextSchedule, eth.angleToTimeUs(180 - cylinderOne - timing));
}
