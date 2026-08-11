/**
 * @file test_idle_hardware.cpp
 *
 * Coverage for applyIACposition(): single solenoid vs double solenoid
 * (Subaru/BMW style "three wire") idle valve duty split, plus initIdleHardware()
 * stepper branches: step/dir driver, dual H-bridge and raw-output stepper - the
 * latter two claim DC motor pool slots ETB_COUNT+0/1, see
 * https://github.com/rusefi/rusefi/pull/9466 for the planned pool split.
 */

#include "pch.h"

#include "idle_hardware.h"
#include "dc_motors.h"
#include "stepper.h"

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

TEST(Actuators, IdleSingleSolenoidInit) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->useStepperIdle = false;
	engineConfiguration->isDoubleSolenoidIdle = false;
	engineConfiguration->idle.solenoidPin = Gpio::A7;
	engineConfiguration->idle.solenoidFrequency = 200;

	EXPECT_NO_FATAL_ERROR(initIdleHardware());

	engine->timeToStopIdleTest = getTimeNowUs() + MS2US(3000);
	applyIACposition(30);
	EXPECT_NEAR(0.30f, getIdleSolenoidOpenDutyForUnitTest(), EPS4D);
}

TEST(Actuators, IdleDoubleSolenoidRequiresSecondPin) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->useStepperIdle = false;
	engineConfiguration->isDoubleSolenoidIdle = true;
	engineConfiguration->idle.solenoidPin = Gpio::A7;
	engineConfiguration->idle.solenoidFrequency = 200;
	engineConfiguration->secondSolenoidPin = Gpio::Unassigned;

	EXPECT_FATAL_ERROR(initIdleHardware());
}

TEST(Actuators, StepDirStepperIdle) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	Sensor::setMockValue(SensorType::BatteryVoltage, 12);

	engineConfiguration->useStepperIdle = true;
	engineConfiguration->useHbridgesToDriveIdleStepper = false;
	engineConfiguration->useRawOutputToDriveIdleStepper = false;
	engineConfiguration->idleStepperTotalSteps = 200;
	engineConfiguration->idle.stepperStepPin = Gpio::C13;
	engineConfiguration->idle.stepperDirectionPin = Gpio::C14;

	EXPECT_NO_FATAL_ERROR(initIdleHardware());

	auto& iac = getIacMotorForUnitTest();

	// first iteration latches the initial (saved = 0) position, no parking needed
	iac.doIteration();
	EXPECT_EQ(0, iac.m_currentPosition);

	// 25% of 200 steps
	applyIACposition(25);
	EXPECT_FLOAT_EQ(50, iac.getTargetPosition());

	// the motor works its way toward the target one step per iteration
	iac.doIteration();
	iac.doIteration();
	iac.doIteration();
	EXPECT_EQ(3, iac.m_currentPosition);
	EXPECT_TRUE(isIdleMotorBusy());
}

// full-step "two phase on" quadrature sequence of DualHBridgeStepper, see stepper_dual_hbridge.cpp
static void expectHbridgePhase(int phase) {
	static const float phaseA[] = { 1, 1, -1, -1 };
	static const float phaseB[] = { -1, 1, 1, -1 };

	// legacy invariant of the shared DC pool: the idle stepper is wired to the two
	// slots right after the ETBs
	DcMotor* motorA = getDcMotorForUnitTest(ETB_COUNT + 0);
	DcMotor* motorB = getDcMotorForUnitTest(ETB_COUNT + 1);

	EXPECT_FLOAT_EQ(phaseA[phase & 3], motorA->get()) << "phase A at step " << phase;
	EXPECT_FLOAT_EQ(phaseB[phase & 3], motorB->get()) << "phase B at step " << phase;
}

static void runHbridgeStepperIdleScenario() {
	Sensor::setMockValue(SensorType::BatteryVoltage, 12);

	engineConfiguration->useStepperIdle = true;
	engineConfiguration->idleStepperTotalSteps = 4;
	engineConfiguration->stepperNumMicroSteps = STEPPER_FULL;
	// force deterministic parking so the test does not depend on saved position
	engineConfiguration->stepperForceParkingEveryRestart = true;
	engineConfiguration->stepperParkingExtraSteps = 0;

	EXPECT_NO_FATAL_ERROR(initIdleHardware());

	auto& iac = getIacMotorForUnitTest();

	// motors untouched until the first iteration
	EXPECT_FLOAT_EQ(0, getDcMotorForUnitTest(ETB_COUNT + 0)->get());
	EXPECT_FLOAT_EQ(0, getDcMotorForUnitTest(ETB_COUNT + 1)->get());

	// first iteration parks the valve: idleStepperTotalSteps reverse steps back to phase 0
	iac.doIteration();
	EXPECT_EQ(0, iac.m_currentPosition);
	expectHbridgePhase(0);

	// 100% -> target is idleStepperTotalSteps
	applyIACposition(100);
	EXPECT_FLOAT_EQ(4, iac.getTargetPosition());

	// each iteration advances the quadrature sequence by exactly one step
	for (int step = 1; step <= 4; step++) {
		iac.doIteration();
		EXPECT_EQ(step, iac.m_currentPosition);
		expectHbridgePhase(step);
		EXPECT_TRUE(isIdleMotorBusy());
	}

	// once on target the motor goes to sleep and reports not busy
	iac.doIteration();
	EXPECT_EQ(4, iac.m_currentPosition);
	EXPECT_FALSE(isIdleMotorBusy());
}

TEST(Actuators, HbridgeStepperIdleDrivesDcPoolSlots) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->useHbridgesToDriveIdleStepper = true;

	runHbridgeStepperIdleScenario();
}

TEST(Actuators, RawOutputStepperIdleDrivesDcPoolSlots) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// four push-pull outputs directly driving the stepper coils
	engineConfiguration->useRawOutputToDriveIdleStepper = true;
	engineConfiguration->stepper_raw_output[0] = Gpio::D0;
	engineConfiguration->stepper_raw_output[1] = Gpio::D1;
	engineConfiguration->stepper_raw_output[2] = Gpio::D2;
	engineConfiguration->stepper_raw_output[3] = Gpio::D3;

	runHbridgeStepperIdleScenario();
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
