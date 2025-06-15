#include "pch.h"
#include "spark_logic.h"
#include "harley.h"
#include "fuel_math.h"
#include "defaults.h"
#include "util/injection_crank_helper.h"

	// let's pretend to have a 32 degree V odd fire engine.
static const float cylinderOne = -19;
static const float cylinderTwo = 13;
static const angle_t timing = 1; // same timing cranking and running modes

using ::testing::_;

static void configureOddFiringEngine(EngineTestHelper &eth) {
	engineConfiguration->cranking.rpm = 100;
	engineConfiguration->timing_offset_cylinder[0] = cylinderOne;
	engineConfiguration->timing_offset_cylinder[1] = cylinderTwo;
	setTable(config->ignitionTable, timing); // this changes run mode timing
	engineConfiguration->crankingTimingAngle = timing;
	engine->tdcMarkEnabled = false; // reduce event queue noise TODO extract helper method
}

#if FUEL_RPM_COUNT == 16
TEST(OddFireRunningMode, hd) {
  // basic engine setup
	EngineTestHelper eth(engine_type_e::HARLEY);
	configureOddFiringEngine(eth);
	extern bool unitTestBusyWaitHack;
	unitTestBusyWaitHack = true;
	engineConfiguration->vvtMode[0] = VVT_SINGLE_TOOTH; // need to avoid engine phase sync requirement
	setTestFuelCrankingTable(27);

  // we need some fuel duration so let's mock airmass just to have legit fuel, we do not care for amount here at all
	EXPECT_CALL(*eth.mockAirmass, getAirmass(/*any rpm*/_, _))
		.WillRepeatedly(Return(AirmassResult{0.1008f, 50.0f}));

	engineConfiguration->camInputs[0] = Gpio::Unassigned;
	engineConfiguration->overrideTriggerGaps = false;

	eth.setTriggerType(trigger_type_e::TT_HALF_MOON);
	// end of configuration

  // send fake crank signal events so that trigger handler schedules actuators
	eth.fireTriggerEvents2(2 /* count */ , 60 /* ms */);
	LimpState limitedSparkState = getLimpManager()->allowIgnition();
	ASSERT_TRUE(limitedSparkState.value);
	ASSERT_EQ(limitedSparkState.reason, ClearReason::None);
	ASSERT_EQ(IM_SIMULTANEOUS, getCurrentInjectionMode()); // still spinning up
	ASSERT_NEAR(0.027, getInjectionMass(200), EPS3D);

//	ASSERT_NEAR(-528, eth.timeToAngle(-176), EPS3D);
//	ASSERT_NEAR(-250.778, eth.timeToAngle(-83.593), EPS3D);
//	ASSERT_NEAR(-220.0, eth.timeToAngle(-73.333333), EPS3D);
//	ASSERT_NEAR(?0.0, eth.timeToAngle(-70.66666), EPS3D);
//	ASSERT_NEAR(-200.0, eth.timeToAngle(-66.66666), EPS3D);
//	ASSERT_NEAR(-168.0, eth.timeToAngle(-56.0), EPS3D);
//	ASSERT_NEAR(32, eth.timeToAngle(10.66666666), EPS3D);
	//ASSERT_NEAR(?, eth.timeToAngle(40.3333), EPS3D);
//	ASSERT_NEAR(140, eth.timeToAngle(46.66666), EPS3D);
//	ASSERT_NEAR(141.222, eth.timeToAngle(47.074), EPS3D);
//	ASSERT_NEAR(160.0, eth.timeToAngle(53.333333), EPS3D);

	angle_t expectedAngle3 = -180 + cylinderOne - timing;

	ASSERT_EQ( 8,  engine->scheduler.size());
	auto const fireSparkAndPrepareNextScheduleAction{ action_s::make<fireSparkAndPrepareNextSchedule>((IgnitionEvent*){})};
	eth.assertEvent5("spark down#3", 3, fireSparkAndPrepareNextScheduleAction, eth.angleToTimeUs(expectedAngle3));

	angle_t expectedAngle5 = -180 + cylinderTwo - timing;
	eth.assertEvent5("spark down#5", 5, fireSparkAndPrepareNextScheduleAction, eth.angleToTimeUs(expectedAngle5));

	ASSERT_EQ(500, Sensor::getOrZero(SensorType::Rpm));

	engine->scheduler.executeAll(getTimeNowUs() + MS2US(1000000));

	eth.fireTriggerEvents2(2 /* count */ , 60 /* ms */);
	ASSERT_EQ(IM_SEQUENTIAL, getCurrentInjectionMode());
	ASSERT_NEAR(0.0069257142022, getInjectionMass(200), EPS3D);

	ASSERT_EQ( 8,  engine->scheduler.size());
	auto const turnInjectionPinLowAction{ action_s::make<turnInjectionPinLow>((InjectionEvent*){})};
	eth.assertEvent5("fuel down2#2", 2, turnInjectionPinLowAction, eth.angleToTimeUs(180 + PORT_INJECTION_OFFSET + cylinderOne));
	eth.assertEvent5("spark down2#4", 4, fireSparkAndPrepareNextScheduleAction, eth.angleToTimeUs(-180 + cylinderOne - timing));
	eth.assertEvent5("fuel down2#7", 7, turnInjectionPinLowAction, eth.angleToTimeUs(540 + PORT_INJECTION_OFFSET + cylinderTwo));
	eth.assertEvent5("spark down2#0", 0, fireSparkAndPrepareNextScheduleAction, eth.angleToTimeUs(-540 + cylinderTwo - timing));

	ASSERT_EQ(2, engine->getBailedOnDwellCount()) << "Please check if our dwell algorithm have really got better.";
}
#endif //FUEL_RPM_COUNT == 16