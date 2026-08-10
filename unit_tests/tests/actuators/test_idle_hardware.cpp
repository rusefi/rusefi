/**
 * @file test_idle_hardware.cpp
 *
 * Coverage for applyIACposition(): single solenoid vs double solenoid
 * (Subaru/BMW style "three wire") idle valve duty split.
 */

#include "pch.h"

#include "idle_hardware.h"

TEST(Actuators, IdleSingleSolenoid) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->isDoubleSolenoidIdle = false;
	// pretend a bench test is in progress so the valve is driven even while the engine is stopped
	engine->timeToStopIdleTest = getTimeNowUs() + MS2US(3000);

	applyIACposition(30);
	EXPECT_NEAR(0.30f, getIdleSolenoidOpenDutyForUnitTest(), EPS4D);

	applyIACposition(75);
	EXPECT_NEAR(0.75f, getIdleSolenoidOpenDutyForUnitTest(), EPS4D);
}

TEST(Actuators, IdleDoubleSolenoid) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->isDoubleSolenoidIdle = true;
	engine->timeToStopIdleTest = getTimeNowUs() + MS2US(3000);

	// duty is mapped into the 0.01..0.99 range: open = 0.01 + 0.98 * duty, close is complementary
	applyIACposition(30);
	EXPECT_NEAR(0.304f, getIdleSolenoidOpenDutyForUnitTest(), EPS4D);
	EXPECT_NEAR(0.696f, getIdleSolenoidCloseDutyForUnitTest(), EPS4D);

	// near the extremes both coils keep switching - neither goes to DC on/off
	applyIACposition(1);
	EXPECT_NEAR(0.0198f, getIdleSolenoidOpenDutyForUnitTest(), EPS4D);
	EXPECT_NEAR(0.9802f, getIdleSolenoidCloseDutyForUnitTest(), EPS4D);

	// at EXACTLY 0%/100% float rounding pushes the nominal 0.01/0.99 endpoints just past
	// ZERO_PWM_THRESHOLD/FULL_PWM_THRESHOLD, so the coils degrade to plain digital on/off
	applyIACposition(0);
	EXPECT_EQ(0, getIdleSolenoidOpenDutyForUnitTest());
	EXPECT_EQ(1, getIdleSolenoidCloseDutyForUnitTest());

	applyIACposition(100);
	EXPECT_EQ(1, getIdleSolenoidOpenDutyForUnitTest());
	EXPECT_EQ(0, getIdleSolenoidCloseDutyForUnitTest());
}

TEST(Actuators, IdleSolenoidsOffWhileEngineStopped) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->isDoubleSolenoidIdle = true;

	// while a bench test is active the valves are driven despite the engine being stopped
	engine->timeToStopIdleTest = getTimeNowUs() + MS2US(3000);
	applyIACposition(50);
	EXPECT_NEAR(0.50f, getIdleSolenoidOpenDutyForUnitTest(), EPS4D);
	EXPECT_NEAR(0.50f, getIdleSolenoidCloseDutyForUnitTest(), EPS4D);

	// engine stopped and no bench test -> both solenoids off to be quieter and save power
	engine->timeToStopIdleTest = 0;
	applyIACposition(50);
	EXPECT_EQ(0, getIdleSolenoidOpenDutyForUnitTest());
	EXPECT_EQ(0, getIdleSolenoidCloseDutyForUnitTest());
}
