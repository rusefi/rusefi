#include "pch.h"

#include "vvt.h"
#include "defaults.h"

using ::testing::StrictMock;
using ::testing::Return;

TEST(Vvt, TestSetPoint) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// Set up a mock target map & pwm output
	StrictMock<MockVp3d> targetMap;
	StrictMock<MockPwm> pwm;

	EXPECT_CALL(targetMap, getValue(1500, 55))
		.WillRepeatedly(Return(20)); // one from onFastCallback() then getSetpoint()
	EXPECT_CALL(pwm, setSimplePwmDutyCycle(0.730005));

	// set up VVT config
	engineConfiguration->vvtActivationDelayMs = 5;
	engineConfiguration->vvtControlMinRpm = 500;

	// mock RPM
	engine->rpmCalculator.setRpmValue(1500);
	ASSERT_EQ(1500, Sensor::getOrZero(SensorType::Rpm));
	ASSERT_TRUE(engine->rpmCalculator.isRunning());
	advanceTimeUs(0.5e6);

	VvtController dut(0);
	dut.init(&targetMap, &pwm);

	// Mock necessary inputs
	engine->engineState.fuelingLoad = 55;
	engine->triggerCentral.vvtPositionUpdateTimer[0][0].reset();

	// update m_engineRunningLongEnough / m_isRpmHighEnough flags
	dut.onFastCallback();
	// Test dut
	EXPECT_EQ(20, dut.getSetpoint().value_or(0));
}

struct FakeMap : public ValueProvider3D {
	float setpoint = 0;

	float getValue(float xColumn, float yRow) const override {
		return setpoint;
	}
};

TEST(Vvt, ActivationDelayRetainsFractionalSeconds) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	Sensor::setMockValue(SensorType::Clt, 70);
	engineConfiguration->vvtControlMinClt = 0;
	engineConfiguration->vvtControlMinRpm = 500;
	engineConfiguration->vvtActivationDelayMs = 1500;
	engine->rpmCalculator.setRpmValue(1500);

	FakeMap targetMap;
	targetMap.setpoint = 20;
	StrictMock<MockPwm> pwm;
	EXPECT_CALL(pwm, setSimplePwmDutyCycle(0)).Times(3);
	VvtController dut(0);
	dut.init(&targetMap, &pwm);

	advanceTimeUs(900000);
	dut.onFastCallback();
	EXPECT_FALSE(dut.getSetpoint());

	advanceTimeUs(350000);
	dut.onFastCallback();
	EXPECT_FALSE(dut.getSetpoint());

	advanceTimeUs(350000);
	dut.onFastCallback();
	EXPECT_EQ(20, dut.getSetpoint().value_or(-1000));
}

TEST(VVT, SetpointHysteresisAdvancingCam) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	Sensor::setMockValue(SensorType::Clt, 70); // m_isCltWarmEnough
	// m_isRpmHighEnough
	engine->rpmCalculator.setRpmValue(1500);
	engineConfiguration->vvtControlMinRpm = 500;
	// m_engineRunningLongEnough
	advanceTimeUs(0.8e6);
	engineConfiguration->vvtActivationDelayMs = 5;
	engineConfiguration->invertVvtControlIntake = false;
	engineConfiguration->invertVvtControlExhaust = false;

	FakeMap targetMap;
	MockPwm pwm;

	VvtController dut(0);
	dut.init(&targetMap, &pwm);

	// update m_engineRunningLongEnough / m_isRpmHighEnough flags
	dut.onFastCallback();

	// 0 position returns unexpected
	targetMap.setpoint = 0;
	EXPECT_EQ(-1000, dut.getSetpoint().value_or(-1000));

	// Between hysteresis still return unexpected
	targetMap.setpoint = 2;
	EXPECT_EQ(-1000, dut.getSetpoint().value_or(-1000));

	// Above hysteresis returns real value
	targetMap.setpoint = 5;
	EXPECT_EQ(5, dut.getSetpoint().value_or(-1000));

	// Between hysteresis still returns valid
	targetMap.setpoint = 2;
	EXPECT_EQ(2, dut.getSetpoint().value_or(-1000));

	// Back under hysteresis retuns unexpected again
	targetMap.setpoint = 0.5;
	EXPECT_EQ(-1000, dut.getSetpoint().value_or(-1000));
}

TEST(VVT, SetpointHysteresisRetardingCam) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	Sensor::setMockValue(SensorType::Clt, 70); // m_isCltWarmEnough
	// m_isRpmHighEnough
	engine->rpmCalculator.setRpmValue(1500);
	engineConfiguration->vvtControlMinRpm = 500;
	// m_engineRunningLongEnough
	advanceTimeUs(0.8e6);
	engineConfiguration->vvtActivationDelayMs = 5;

	engineConfiguration->invertVvtControlIntake = true;
	engineConfiguration->invertVvtControlExhaust = false;

	FakeMap targetMap;
	MockPwm pwm;

	VvtController dut(0);
	dut.init(&targetMap, &pwm);

	// update m_engineRunningLongEnough / m_isRpmHighEnough flags
	dut.onFastCallback();

	// 0 position returns unexpected
	targetMap.setpoint = 0;
	EXPECT_EQ(-1000, dut.getSetpoint().value_or(-1000));

	// Between hysteresis still return unexpected
	targetMap.setpoint = -2;
	EXPECT_EQ(-1000, dut.getSetpoint().value_or(-1000));

	// Above hysteresis returns real value
	targetMap.setpoint = -5;
	EXPECT_EQ(-5, dut.getSetpoint().value_or(-1000));

	// Between hysteresis still returns valid
	targetMap.setpoint = -2;
	EXPECT_EQ(-2, dut.getSetpoint().value_or(-1000));

	// Back under hysteresis retuns unexpected again
	targetMap.setpoint = -0.5;
	EXPECT_EQ(-1000, dut.getSetpoint().value_or(-1000));
}

TEST(VVT, ObservePlant) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engine->triggerCentral.vvtPosition[0][0] = 23;
	engine->triggerCentral.vvtPositionUpdateTimer[0][0].reset();

	VvtController dut(0);
	dut.init(nullptr, nullptr);

	EXPECT_EQ(23, dut.observePlant().value_or(0));
}

TEST(Vvt, openLoop) {
	VvtController dut(0);

	// No open loop for now
	EXPECT_EQ(dut.getOpenLoop(10), 0);
}

TEST(Vvt, ClosedLoopNotInverted) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	VvtController dut(/* second cam on second bank*/3);
	int camIndex = 1;
	ASSERT_EQ(dut.getCamIndex(), camIndex);
	dut.init(nullptr, nullptr);

	engineConfiguration->auxPid[camIndex].pFactor = 1.5f;
	engineConfiguration->auxPid[camIndex].iFactor = 0;
	engineConfiguration->auxPid[camIndex].dFactor = 0;
	engineConfiguration->auxPid[camIndex].offset = 0;

	// Target of 30 with position 20 should yield positive duty, P=1.5 means 15% duty for 10% error
	EXPECT_EQ(dut.getClosedLoop(30, 20).value_or(0), 15);
}

TEST(Vvt, ClosedLoopInverted) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	VvtController dut(/*first cam on second bank*/2);
	int camIndex = 0;
	ASSERT_EQ(dut.getCamIndex(), camIndex);
	dut.init(nullptr, nullptr);

	engineConfiguration->invertVvtControlIntake = true;
	engineConfiguration->auxPid[camIndex].pFactor = 1.5f;
	engineConfiguration->auxPid[camIndex].iFactor = 0;
	engineConfiguration->auxPid[camIndex].dFactor = 0;
	engineConfiguration->auxPid[camIndex].offset = 0;

	// Target of -30 with position -20 should yield positive duty, P=1.5 means 15% duty for 10% error
	EXPECT_EQ(dut.getClosedLoop(-30, -20).value_or(0), 15);
}

TEST(VVT, SetpointHysteresisSkippedInAdvancedMode) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	Sensor::setMockValue(SensorType::Clt, 70); // m_isCltWarmEnough
	// m_isRpmHighEnough
	engine->rpmCalculator.setRpmValue(1500);
	engineConfiguration->vvtControlMinRpm = 500;
	// m_engineRunningLongEnough
	advanceTimeUs(0.8e6);
	engineConfiguration->vvtActivationDelayMs = 5;
	engineConfiguration->invertVvtControlIntake = false;
	engineConfiguration->invertVvtControlExhaust = false;

	engineConfiguration->vvtAdvancedModeEnabled = true;

	FakeMap targetMap;
	MockPwm pwm;

	VvtController dut(0);
	dut.init(&targetMap, &pwm);

	// update m_engineRunningLongEnough / m_isRpmHighEnough flags
	dut.onFastCallback();

	// In Advanced Mode, even a 0 (or near-0) target flows through to the feedforward+PID math
	// instead of being disabled by the near-rest hysteresis gate.
	targetMap.setpoint = 0;
	EXPECT_EQ(0, dut.getSetpoint().value_or(-1000));

	targetMap.setpoint = 0.5;
	EXPECT_EQ(0.5, dut.getSetpoint().value_or(-1000));

	targetMap.setpoint = -0.5;
	EXPECT_EQ(-0.5, dut.getSetpoint().value_or(-1000));
}

// VVT Advanced Mode: the feedforward baseline is always the duty-vs-oil-pressure curve (the fixed
// "Hold Duty" / pid_s.offset is never used while Advanced Mode is enabled). On top of that, the
// P+I+D trim's P factor comes from a distance-vs-P-factor curve instead of the fixed pid_s.pFactor
// -- I and D still use the fixed auxPid iFactor/dFactor. The trim always runs on top of the
// feedforward; if I trim isn't wanted, set the fixed auxPid iFactor to 0.

static void setupAdvancedIntakePid(int camIndex) {
	engineConfiguration->auxPid[camIndex].pFactor = 0;
	engineConfiguration->auxPid[camIndex].iFactor = 0;
	engineConfiguration->auxPid[camIndex].dFactor = 0;
	engineConfiguration->auxPid[camIndex].offset = 0;
	engineConfiguration->auxPid[camIndex].periodMs = 10;
	engineConfiguration->auxPid[camIndex].minValue = -1000;
	engineConfiguration->auxPid[camIndex].maxValue = 1000;
}

// Flat (all-zero) curves matching the real bin counts/ranges: a symmetric -40..40 deg distance
// axis (9 points, zero at the center, bin index 5 at +10 deg) driving P factor, and a 0..500 kPa
// oil pressure axis (6 points, bin index 3 at 300 kPa) driving base duty. Individual tests
// override specific bins as needed.
static void setupAdvancedIntakeCurvesFlat(engine_configuration_s* d) {
	for (size_t i = 0; i < efi::size(d->vvtAdvDistanceBinsIntake); i++) {
		d->vvtAdvDistanceBinsIntake[i] = -40.0f + i * 10.0f;
		d->vvtAdvPFactorIntake[i] = 0;
	}
	for (size_t i = 0; i < efi::size(d->vvtAdvOilPressureBinsIntake); i++) {
		d->vvtAdvOilPressureBinsIntake[i] = i * 100.0f;
		d->vvtAdvDutyIntake[i] = 0;
	}
}

TEST(Vvt, AdvancedModeDisabledMatchesLegacyBehavior) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	VvtController dut(0);
	int camIndex = 0;
	dut.init(nullptr, nullptr);

	engineConfiguration->vvtAdvancedModeEnabled = false;

	engineConfiguration->auxPid[camIndex].pFactor = 1.5f;
	engineConfiguration->auxPid[camIndex].iFactor = 0;
	engineConfiguration->auxPid[camIndex].dFactor = 0;
	engineConfiguration->auxPid[camIndex].offset = 0;

	// Same as ClosedLoopNotInverted: P=1.5 means 15% duty for 10% error.
	EXPECT_EQ(dut.getClosedLoop(30, 20).value_or(0), 15);
}

TEST(Vvt, AdvancedModeFeedforwardTracksOilPressureNotHoldDuty) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	VvtController dut(0);
	int camIndex = 0;
	dut.init(nullptr, nullptr);
	setupAdvancedIntakePid(camIndex);
	engineConfiguration->auxPid[camIndex].offset = 999.0f; // Hold Duty -- must never leak in

	auto* d = engineConfiguration;
	d->vvtAdvancedModeEnabled = true;
	setupAdvancedIntakeCurvesFlat(d);
	d->vvtAdvDutyIntake[0] = 7.0f;  // 0 kPa
	d->vvtAdvDutyIntake[3] = 42.0f; // 300 kPa

	// Oil pressure = 0 kPa (no sensor mocked yet) -> feedforward is the curve's leftmost value.
	Sensor::resetMockValue(SensorType::OilPressure);
	EXPECT_NEAR(dut.getClosedLoop(20, 20).value_or(0), 7.0f, 0.01f);

	// 300 kPa -> same curve, different point.
	Sensor::setMockValue(SensorType::OilPressure, 300);
	EXPECT_NEAR(dut.getClosedLoop(20, 20).value_or(0), 42.0f, 0.01f);
}

TEST(Vvt, AdvancedModeUsesCurvePFactor) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	VvtController dut(0);
	int camIndex = 0;
	dut.init(nullptr, nullptr);
	setupAdvancedIntakePid(camIndex);
	engineConfiguration->auxPid[camIndex].pFactor = 999.0f; // fixed P factor -- must never leak in
	engineConfiguration->auxPid[camIndex].offset = 999.0f;  // Hold Duty -- must never leak in

	auto* d = engineConfiguration;
	d->vvtAdvancedModeEnabled = true;
	setupAdvancedIntakeCurvesFlat(d); // zero P-factor and zero duty curves everywhere
	d->vvtAdvPFactorIntake[5] = 1.0f; // bin index 5 = +10 deg -> P factor 1.0 there

	// distance = 10 deg -> PID runs using the curve's P factor (1.0) at that distance, not the
	// fixed (sentinel) pFactor; feedforward is still the (zero) duty curve.
	EXPECT_NEAR(dut.getClosedLoop(30, 20).value_or(0), 10.0f, 0.01f);
}

TEST(Vvt, DistanceLiveDataOnlyTrackedInAdvancedMode) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	VvtController dut(0);
	int camIndex = 0;
	dut.init(nullptr, nullptr);
	setupAdvancedIntakePid(camIndex);

	auto* d = engineConfiguration;

	// Basic mode: distance tracker is not written.
	d->vvtAdvancedModeEnabled = false;
	dut.vvtDistance = 0;
	dut.getClosedLoop(30, 20);
	EXPECT_EQ(dut.vvtDistance, 0);

	// Advanced mode: distance tracker reflects target - observation.
	d->vvtAdvancedModeEnabled = true;
	setupAdvancedIntakeCurvesFlat(d);
	dut.getClosedLoop(30, 20);
	EXPECT_NEAR(dut.vvtDistance, 10.0f, 0.5f); // vvtDistance is a scaled int16 (0.1 deg resolution)
}

TEST(VVT, DisabledBelowMinRpmEvenInAdvancedMode) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	Sensor::setMockValue(SensorType::Clt, 70); // m_isCltWarmEnough
	engineConfiguration->vvtControlMinRpm = 500;
	engineConfiguration->vvtActivationDelayMs = 5;
	engineConfiguration->invertVvtControlIntake = false;
	engineConfiguration->invertVvtControlExhaust = false;

	engineConfiguration->vvtAdvancedModeEnabled = true;

	FakeMap targetMap;
	MockPwm pwm;

	VvtController dut(0);
	dut.init(&targetMap, &pwm);
	targetMap.setpoint = 20; // well above the near-rest hysteresis band

	// Bring the engine into RUNNING state once (RPM above the default cranking.rpm) and let
	// m_engineRunningLongEnough settle. RpmCalculator::setRpmValue() only resets its "engine
	// started" timer on a transition *into* RUNNING -- once RUNNING, later RPM changes (even below
	// cranking.rpm) don't revert the state or reset the timer, so this isolates the vvtControlMinRpm
	// check below from that unrelated state machine.
	engine->rpmCalculator.setRpmValue(1500);
	advanceTimeUs(0.8e6); // m_engineRunningLongEnough

	// Below vvtControlMinRpm: still fully disabled (getSetpoint() -> unexpected), same as legacy
	// PID mode: Advanced Mode does not bypass the minimum-RPM gate.
	engine->rpmCalculator.setRpmValue(400);
	dut.onFastCallback();
	EXPECT_EQ(-1000, dut.getSetpoint().value_or(-1000));

	// Above it, control resumes as normal.
	engine->rpmCalculator.setRpmValue(1500);
	dut.onFastCallback();
	EXPECT_EQ(20, dut.getSetpoint().value_or(-1000));
}

TEST(Vvt, AdvancedModeDefaultsAndOldTuneMigration) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	EXPECT_FALSE(engineConfiguration->vvtAdvancedModeEnabled);
	EXPECT_FLOAT_EQ(engineConfiguration->vvtAdvDistanceBinsIntake[0], -40);
	EXPECT_FLOAT_EQ(engineConfiguration->vvtAdvDistanceBinsExhaust[4], 0);
	EXPECT_FLOAT_EQ(engineConfiguration->vvtAdvDistanceBinsIntake[8], 40);
	EXPECT_FLOAT_EQ(engineConfiguration->vvtAdvOilPressureBinsExhaust[5], 1000);
	EXPECT_FLOAT_EQ(engineConfiguration->vvtAdvDutyIntake[3], 0);
	EXPECT_FLOAT_EQ(engineConfiguration->vvtAdvPFactorExhaust[5], 0);

	setArrayValues(engineConfiguration->vvtAdvDistanceBinsIntake, 0);
	setArrayValues(engineConfiguration->vvtAdvOilPressureBinsExhaust, 0);
	engineConfiguration->vvtIntake_iTermMin = 0;
	engineConfiguration->vvtIntake_iTermMax = 0;
	EXPECT_TRUE(applyDefaultsOrFixAfterBurn(nullptr));
	EXPECT_FLOAT_EQ(engineConfiguration->vvtAdvDistanceBinsIntake[4], 0);
	EXPECT_FLOAT_EQ(engineConfiguration->vvtAdvDistanceBinsIntake[8], 40);
	EXPECT_FLOAT_EQ(engineConfiguration->vvtAdvOilPressureBinsExhaust[5], 1000);
	EXPECT_EQ(engineConfiguration->vvtIntake_iTermMin, -1000);
	EXPECT_EQ(engineConfiguration->vvtIntake_iTermMax, 1000);

	// A zero on just one side is a valid limit, not an uninitialized pair.
	engineConfiguration->vvtIntake_iTermMin = 0;
	engineConfiguration->vvtIntake_iTermMax = 20;
	engineConfiguration->vvtControlMinRpm = 300;
	engineConfiguration->cranking.rpm = 500;
	applyDefaultsOrFixAfterBurn(nullptr);
	EXPECT_EQ(engineConfiguration->vvtIntake_iTermMin, 0);
	EXPECT_EQ(engineConfiguration->vvtIntake_iTermMax, 20);
	EXPECT_EQ(engineConfiguration->vvtControlMinRpm, 300);
}

// [tag:vvt_advanced_control] Signed gain lookup and telemetry follow solenoid direction.
TEST(Vvt, AdvancedModeCamDirectionCurvesAndTelemetry) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->vvtAdvancedModeEnabled = true;
	engineConfiguration->invertVvtControlIntake = false;
	engineConfiguration->invertVvtControlExhaust = true;
	setupAdvancedIntakePid(0);
	setupAdvancedIntakePid(1);
	setArrayValues(engineConfiguration->vvtAdvDutyIntake, 20);
	setArrayValues(engineConfiguration->vvtAdvDutyExhaust, 50);
	setArrayValues(engineConfiguration->vvtAdvPFactorIntake, 2);
	setArrayValues(engineConfiguration->vvtAdvPFactorExhaust, 0);
	engineConfiguration->vvtAdvPFactorExhaust[3] = 1; // -10 degrees
	engineConfiguration->vvtAdvPFactorExhaust[5] = 3; // +10 degrees
	for (int index = 0; index < CAM_INPUTS_COUNT; index++) {
		VvtController dut(index);
		dut.init(nullptr, nullptr);
		bool intake = dut.getCamIndex() == 0;
		EXPECT_FLOAT_EQ(dut.getClosedLoop(30, 20).value_or(-1000), 40);
		EXPECT_FLOAT_EQ(dut.vvtDistance, intake ? 10 : -10);
		auto& status = engine->outputChannels.vvtStatus[index];
		EXPECT_FLOAT_EQ(status.pTerm, intake ? 20 : -10);
		EXPECT_FLOAT_EQ(status.output, 40);
		EXPECT_FLOAT_EQ(dut.getClosedLoop(20, 30).value_or(-1000), intake ? 0 : 80);
		EXPECT_FLOAT_EQ(dut.vvtDistance, intake ? -10 : 10);
		EXPECT_FLOAT_EQ(status.pTerm, intake ? -20 : 30);
	}
}

TEST(Vvt, AdvancedModeInterpolatesPressureAndHandlesInvalidSensor) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->vvtAdvancedModeEnabled = true;
	setupAdvancedIntakePid(0);
	setLinearCurve(engineConfiguration->vvtAdvOilPressureBinsIntake, 0, 500, 1);
	for (size_t i = 0; i < VVT_ADV_OILP_SIZE; i++) {
		engineConfiguration->vvtAdvDutyIntake[i] = 10 + i * 10;
	}
	VvtController dut(0);
	dut.init(nullptr, nullptr);
	Sensor::setMockValue(SensorType::OilPressure, 250);
	EXPECT_FLOAT_EQ(dut.getClosedLoop(20, 20).value_or(0), 35);
	Sensor::setInvalidMockValue(SensorType::OilPressure);
	EXPECT_FLOAT_EQ(dut.getClosedLoop(20, 20).value_or(0), 10);
}

TEST(Vvt, IntegralBoundsApplyInBothModesAndResetOnBurn) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	for (bool advanced : {false, true}) {
		for (int cam = 0; cam < CAMS_PER_BANK; cam++) {
			engineConfiguration->vvtAdvancedModeEnabled = advanced;
			setupAdvancedIntakePid(cam);
			engineConfiguration->auxPid[cam].iFactor = 100;
			engineConfiguration->vvtIntake_iTermMin = -2;
			engineConfiguration->vvtIntake_iTermMax = 3;
			engineConfiguration->vvtExhaust_iTermMin = -4;
			engineConfiguration->vvtExhaust_iTermMax = 5;
			VvtController dut(cam);
			dut.init(nullptr, nullptr);
			for (int i = 0; i < 10; i++) {
				dut.getClosedLoop(20, 10);
			}
			EXPECT_FLOAT_EQ(dut.getClosedLoop(20, 20).value_or(0), cam == 0 ? 3 : 5);
			for (int i = 0; i < 10; i++) {
				dut.getClosedLoop(10, 20);
			}
			EXPECT_FLOAT_EQ(dut.getClosedLoop(20, 20).value_or(0), cam == 0 ? -2 : -4);
			dut.onConfigurationChange(nullptr);
			EXPECT_FLOAT_EQ(dut.getClosedLoop(20, 20).value_or(1000), 0);
		}
	}
}

TEST(Vvt, AdvancedModeClampsFinalCommandAndDrivesPwm) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->vvtAdvancedModeEnabled = true;
	setupAdvancedIntakePid(0);
	engineConfiguration->auxPid[0].minValue = 10;
	engineConfiguration->auxPid[0].maxValue = 90;
	setArrayValues(engineConfiguration->vvtAdvDutyIntake, 50);
	setArrayValues(engineConfiguration->vvtAdvPFactorIntake, 10);
	StrictMock<MockPwm> pwm;
	VvtController dut(0);
	dut.init(nullptr, &pwm);
	auto output = dut.getClosedLoop(30, 20);
	EXPECT_FLOAT_EQ(output.value_or(0), 90);
	EXPECT_FLOAT_EQ(engine->outputChannels.vvtStatus[0].output, 90);
	EXPECT_FLOAT_EQ(engine->outputChannels.vvtStatus[0].pTerm, 100);
	EXPECT_CALL(pwm, setSimplePwmDutyCycle(testing::FloatNear(0.9f, 0.0001f)));
	dut.setOutput(output);
	EXPECT_FLOAT_EQ(dut.vvtOutput, 90);
	EXPECT_FLOAT_EQ(dut.getClosedLoop(20, 30).value_or(0), 10);
	EXPECT_FLOAT_EQ(engine->outputChannels.vvtStatus[0].output, 10);
	EXPECT_CALL(pwm, setSimplePwmDutyCycle(0));
	dut.setOutput(unexpected);
	EXPECT_FLOAT_EQ(dut.vvtOutput, 0);
}

namespace {

void configureVvtFeedbackReproduction() {
	Sensor::setMockValue(SensorType::Clt, 70);
	engineConfiguration->vvtControlMinRpm = 500;
	engineConfiguration->vvtControlMinClt = 0;
	engineConfiguration->vvtActivationDelayMs = 0;
	engineConfiguration->vvtAdvancedModeEnabled = false;
	engineConfiguration->invertVvtControlIntake = false;
	engineConfiguration->auxPid[0].pFactor = 1;
	engineConfiguration->auxPid[0].iFactor = 0;
	engineConfiguration->auxPid[0].dFactor = 0;
	engineConfiguration->auxPid[0].offset = 0;
	engineConfiguration->auxPid[0].minValue = 0;
	engineConfiguration->auxPid[0].maxValue = 100;
}

} // namespace

TEST(Vvt, DoesNotActuateWithoutAnyCamMeasurement) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configureVvtFeedbackReproduction();
	engineConfiguration->camInputs[0] = Gpio::A10;
	engineConfiguration->vvtMode[0] = VVT_SINGLE_TOOTH;
	engine->rpmCalculator.setRpmValue(1500);
	advanceTimeUs(100000);
	ASSERT_EQ(0u, engine->triggerCentral.vvtEventRiseCounter[0]);
	StrictMock<MockVp3d> targetMap;
	StrictMock<MockPwm> pwm;
	EXPECT_CALL(targetMap, getValue(testing::_, testing::_)).WillRepeatedly(Return(20));
	EXPECT_CALL(pwm, setSimplePwmDutyCycle(0));
	VvtController dut(0);
	dut.init(&targetMap, &pwm);
	ASSERT_FALSE(dut.observePlant());
	dut.onFastCallback();
	EXPECT_FLOAT_EQ(0, dut.vvtOutput);
}

TEST(Vvt, StopsDrivingAfterCamPulsesStop) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configureVvtFeedbackReproduction();
	engineConfiguration->globalTriggerAngleOffset = -37;
	engineConfiguration->vvtMode[0] = VVT_SINGLE_TOOTH;
	engineConfiguration->vvtOffsets[0] = 360;
	setCrankOperationMode();
	eth.setTriggerType(trigger_type_e::TT_HALF_MOON);
	engineConfiguration->camInputs[0] = Gpio::A10;
	eth.fireTriggerEvents2(5, 25);
	for (int i = 0; i < 4; i++) {
		eth.moveTimeForwardUs(10000);
		if (i % 2 == 0) {
			hwHandleVvtCamSignal(TriggerValue::RISE, getTimeNowNt(), 0);
		}
		eth.moveTimeForwardUs(15000);
		eth.firePrimaryTriggerRise();
		eth.moveTimeForwardUs(25000);
		eth.firePrimaryTriggerFall();
	}
	ASSERT_TRUE(engine->triggerCentral.triggerState.hasSynchronizedPhase());
	ASSERT_EQ(1200, engine->rpmCalculator.getCachedRpm());
	const auto camEdges = engine->triggerCentral.vvtEventRiseCounter[0];
	ASSERT_GT(camEdges, 0u);
	const float measuredPosition = engine->triggerCentral.getVVTPosition(0, 0);
	ASSERT_NEAR(71, measuredPosition, 0.001f);
	StrictMock<MockVp3d> targetMap;
	StrictMock<MockPwm> pwm;
	EXPECT_CALL(targetMap, getValue(testing::_, testing::_)).WillRepeatedly(Return(measuredPosition + 20));
	{
		testing::InSequence sequence;
		EXPECT_CALL(pwm, setSimplePwmDutyCycle(testing::FloatNear(0.2f, 0.00001f)));
		EXPECT_CALL(pwm, setSimplePwmDutyCycle(0));
	}
	VvtController dut(0);
	dut.init(&targetMap, &pwm);
	dut.onFastCallback();
	// Crank still turns for thirty seconds, but the cam has no more edges.
	eth.fireTriggerEvents2(600, 25);
	ASSERT_EQ(1200, engine->rpmCalculator.getCachedRpm());
	ASSERT_EQ(camEdges, engine->triggerCentral.vvtEventRiseCounter[0]);
	EXPECT_FALSE(dut.observePlant());
	dut.onFastCallback();
	EXPECT_FLOAT_EQ(0, dut.vvtOutput);
}
