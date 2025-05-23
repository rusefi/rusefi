#include "pch.h"

#include "vvt.h"

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

	// update m_engineRunningLongEnough / m_isRpmHighEnough flags
	dut.onFastCallback();
	// Test dut
	EXPECT_EQ(20, dut.getSetpoint().value_or(0));
}

struct FakeMap : public ValueProvider3D {
	float setpoint;

	float getValue(float xColumn, float yRow) const override {
		return setpoint;
	}
};

TEST(VVT, SetpointHysteresisAdvancingCam) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	FakeMap targetMap;

	VvtController dut(0, 0, 0);
	dut.init(&targetMap, nullptr);

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

	engineConfiguration->invertVvtControlIntake = true;

	FakeMap targetMap;

	VvtController dut(0, 0, 0);
	dut.init(&targetMap, nullptr);

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
