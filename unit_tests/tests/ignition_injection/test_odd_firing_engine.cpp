#include "pch.h"
#include "spark_logic.h"
#include "custom_engine.h"
#include "fuel_math.h"
#include "defaults.h"

using ::testing::_;

#define PORT_INJECTION_OFFSET -400.0f

TEST(OddFireRunningMode, hd) {
  // basic engine setup
	EngineTestHelper eth(engine_type_e::PROTEUS_HARLEY);
	engineConfiguration->cranking.rpm = 100;
	engineConfiguration->vvtMode[0] = VVT_SINGLE_TOOTH; // need to avoid engine phase sync requirement
	// let's pretend to have a 32 degree V odd fire engine.
	float cylinderOne = 19;
	float cylinderTwo = 13;
	engineConfiguration->timing_offset_cylinder[0] = cylinderOne;
	engineConfiguration->timing_offset_cylinder[1] = -cylinderTwo;
	angle_t timing = 1;
	setTable(config->ignitionTable, timing); // run mode timing

  // we need some fuel duration so let's mock airmass just to have legit fuel, we do not care for amount here at all
	EXPECT_CALL(*eth.mockAirmass, getAirmass(/*any rpm*/_, _))
		.WillRepeatedly(Return(AirmassResult{0.2008f, 50.0f}));

	engineConfiguration->crankingTimingAngle = timing;
	engine->tdcMarkEnabled = false; // reduce event queue noise TODO extract helper method
	engineConfiguration->camInputs[0] = Gpio::Unassigned;
	eth.setTriggerType(trigger_type_e::TT_ONE);
	// end of configuration

  // send fake crank signal events so that trigger handler schedules actuators
	eth.fireTriggerEvents2(2 /* count */ , 60 /* ms */);
	LimpState limitedSparkState = getLimpManager()->allowIgnition();
	ASSERT_TRUE(limitedSparkState.value);
	ASSERT_EQ(limitedSparkState.reason, ClearReason::None);
	ASSERT_EQ(IM_SIMULTANEOUS, getCurrentInjectionMode()); // still spinning up
	ASSERT_NEAR(0.0135, getInjectionMass(200), EPS3D);

	angle_t expectedAngle3 = -360 + cylinderOne - timing;

	ASSERT_EQ( 12,  engine->executor.size());
	eth.assertEvent5("spark down#1", 1, (void*)fireSparkAndPrepareNextSchedule, eth.angleToTimeUs(expectedAngle3));

	angle_t expectedAngle7 = -180 + cylinderOne - timing;
	eth.assertEvent5("spark down#5", 5, (void*)fireSparkAndPrepareNextSchedule, eth.angleToTimeUs(expectedAngle7));
}
