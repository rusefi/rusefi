#include "pch.h"
#include "spark_logic.h"
#include "fuel_schedule.h"
#include "harley.h"
#include "fuel_math.h"
#include "defaults.h"
#include "util/injection_crank_helper.h"

using ::testing::_;

#if FUEL_RPM_COUNT == 16
	// let's pretend to have a 32 degree V odd fire engine.
static const float cylinderOne = -19;
static const float cylinderTwo = 13;
static const angle_t timing = 1; // same timing cranking and running modes

static void configureOddFiringEngine(EngineTestHelper &eth) {
	engineConfiguration->cranking.rpm = 100;
	engineConfiguration->timing_offset_cylinder[0] = cylinderOne;
	engineConfiguration->timing_offset_cylinder[1] = cylinderTwo;
	setTable(config->ignitionTable, timing); // this changes run mode timing
	engineConfiguration->crankingTimingAngle = timing;
	engine->tdcMarkEnabled = false; // reduce event queue noise TODO extract helper method
}

TEST(OddFireRunningMode, hd) {
  // basic engine setup
	EngineTestHelper eth(engine_type_e::HARLEY);
	configureOddFiringEngine(eth);
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
	eth.assertEvent5("spark down#3", 3, fireSparkAndPrepareNextScheduleAction, static_cast<efitimeus_t>(eth.angleToTimeUs(expectedAngle3)));

	angle_t expectedAngle5 = -180 + cylinderTwo - timing;
	eth.assertEvent5("spark down#5", 5, fireSparkAndPrepareNextScheduleAction, static_cast<efitimeus_t>(eth.angleToTimeUs(expectedAngle5)));

	ASSERT_EQ(500, Sensor::getOrZero(SensorType::Rpm));

	eth.executeAllPreservingTimeUs(getTimeNowUs() + MS2US(1000000));

	eth.fireTriggerEvents2(2 /* count */ , 60 /* ms */);
	ASSERT_EQ(IM_SEQUENTIAL, getCurrentInjectionMode());
	ASSERT_NEAR(0.0069257142022, getInjectionMass(200), EPS3D);

	ASSERT_EQ( 8,  engine->scheduler.size());
	auto const turnInjectionPinLowAction{ action_s::make<endInjection>(scheduler_arg_t{})};
	eth.assertEvent5("fuel down2#2", 2, turnInjectionPinLowAction, static_cast<efitimeus_t>(eth.angleToTimeUs(180 + PORT_INJECTION_OFFSET + cylinderOne)));
	eth.assertEvent5("spark down2#4", 4, fireSparkAndPrepareNextScheduleAction, static_cast<efitimeus_t>(eth.angleToTimeUs(-180 + cylinderOne - timing)));
	eth.assertEvent5("fuel down2#7", 7, turnInjectionPinLowAction, static_cast<efitimeus_t>(eth.angleToTimeUs(540 + PORT_INJECTION_OFFSET + cylinderTwo)));
	eth.assertEvent5("spark down2#0", 0, fireSparkAndPrepareNextScheduleAction, static_cast<efitimeus_t>(eth.angleToTimeUs(-540 + cylinderTwo - timing)));

	ASSERT_EQ(2, engine->getBailedOnDwellCount()) << "Please check if our dwell algorithm have really got better.";
}
#endif //FUEL_RPM_COUNT == 16

// The odd-fire offset of a spark is the offset of the cylinder which is firing. It used to be
// looked up by coil, so with one coil (distributor) every spark got the first cylinder's offset,
// and with two coils or wasted spark only the first cylinders' offsets were used.
static void checkOddFireOffsetFollowsFiringCylinder(ignition_mode_e ignitionMode) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;
	engineConfiguration->ignitionMode = ignitionMode;
	// cylinder 3 fires second, ten degrees late; cylinder 4 fires third, five degrees early
	engineConfiguration->timing_offset_cylinder[2] = 10;
	engineConfiguration->timing_offset_cylinder[3] = -5;

	constexpr float rpm = 1000;
	engine->rpmCalculator.setRpmValue(rpm);
	engine->ignitionState.updateDwell(rpm, false);
	setArrayValues(engine->engineState.timingAdvance, 0.0f);
	initializeIgnitionActions();
	ASSERT_TRUE(engine->ignitionEvents.isReady);

	auto& elements = engine->ignitionEvents.elements;
	EXPECT_NEAR(190, elements[1].sparkAngle - elements[0].sparkAngle, 0.01);
	EXPECT_NEAR(355, elements[2].sparkAngle - elements[0].sparkAngle, 0.01);
	EXPECT_NEAR(540, elements[3].sparkAngle - elements[0].sparkAngle, 0.01);
}

TEST(OddFire, offsetOfFiringCylinderWithSingleCoil) {
	checkOddFireOffsetFollowsFiringCylinder(IM_ONE_COIL);
}

TEST(OddFire, offsetOfFiringCylinderWithTwoCoils) {
	checkOddFireOffsetFollowsFiringCylinder(IM_TWO_COILS);
}

TEST(OddFire, offsetOfFiringCylinderWithIndividualCoils) {
	checkOddFireOffsetFollowsFiringCylinder(IM_INDIVIDUAL_COILS);
}

// The injection angle of an event includes the odd-fire offset of its own cylinder. The event
// used to learn its cylinder only after computing the angle, so the first schedule built after
// start used the first cylinder's offset for every event, and in simultaneous and single point
// modes, where the cylinder was taken to be the first one, every schedule did.
static void checkInjectionOddFireOffset(injection_mode_e mode) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	EXPECT_CALL(*eth.mockAirmass, getAirmass(_, _))
		.WillRepeatedly(Return(AirmassResult{0.1008f, 50.0f}));
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth, mode);
	engineConfiguration->firingOrder = FO_1_3_4_2;
	// cylinder 3 is second in the firing order, cylinder 4 third
	engineConfiguration->timing_offset_cylinder[2] = 10;
	engineConfiguration->timing_offset_cylinder[3] = -5;

	engine->rpmCalculator.setRpmValue(2000);
	engine->periodicFastCallback();
	ASSERT_EQ(mode, getCurrentInjectionMode());

	FuelSchedule dut;
	dut.addFuelEvents();
	ASSERT_TRUE(dut.isReady);
	// right from the first schedule
	auto angleAfterFirst = [&](int index) {
		float angle = dut.elements[index].injectionStartAngle - dut.elements[0].injectionStartAngle;
		wrapAngle(angle, "test", ObdCode::CUSTOM_ERR_6555);
		return angle;
	};
	EXPECT_NEAR(190, angleAfterFirst(1), 0.01);
	EXPECT_NEAR(355, angleAfterFirst(2), 0.01);
	EXPECT_NEAR(540, angleAfterFirst(3), 0.01);

	// and rebuilding it with nothing changed moves nothing
	float firstBuild = dut.elements[1].injectionStartAngle;
	dut.addFuelEvents();
	EXPECT_FLOAT_EQ(firstBuild, dut.elements[1].injectionStartAngle);
}

TEST(OddFire, injectionOffsetOfOwnCylinderSequential) {
	checkInjectionOddFireOffset(IM_SEQUENTIAL);
}

TEST(OddFire, injectionOffsetOfOwnCylinderBatch) {
	checkInjectionOddFireOffset(IM_BATCH);
}

TEST(OddFire, injectionOffsetOfOwnCylinderSimultaneous) {
	checkInjectionOddFireOffset(IM_SIMULTANEOUS);
}
