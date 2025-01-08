#include "pch.h"

#include "vvt.h"

using ::testing::StrictMock;
using ::testing::Return;

TEST(VVT, Setpoint) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// Set up a mock target map
	StrictMock<MockVp3d> targetMap;
	EXPECT_CALL(targetMap, getValue(4321, 55))
		.WillRepeatedly(Return(20));

	// Mock necessary inputs
	engine->engineState.fuelingLoad = 55;
	Sensor::setMockValue(SensorType::Rpm,  4321);

	VvtController dut(0);
	dut.init(&targetMap, nullptr);

	setTimeNowUs(0);

	// Test dut
	EXPECT_EQ(20, dut.getSetpoint().value_or(0));

	// Apply position bump
	dut.setTargetOffset(10);
	EXPECT_EQ(20 + 10, dut.getSetpoint().value_or(0));

	// 1.9 seconds: still bumped
	setTimeNowUs(1.9e6);
	EXPECT_EQ(20 + 10, dut.getSetpoint().value_or(0));

	// 2.1 seconds: things go back to normal
	setTimeNowUs(2.1e6);
	EXPECT_EQ(20, dut.getSetpoint().value_or(0));
}

TEST(VVT, ObservePlant) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engine->triggerCentral.vvtPosition[0][0] = 23;

	VvtController dut(0);
	dut.init(nullptr, nullptr);

	EXPECT_EQ(23, dut.observePlant().value_or(0));
}

TEST(VVT, OpenLoop) {
	VvtController dut(0, 0, 0);

	// No open loop for now
	EXPECT_EQ(dut.getOpenLoop(10), 0);
}

TEST(VVT, ClosedLoopNotInverted) {
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

TEST(VVT, ClosedLoopInverted) {
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
