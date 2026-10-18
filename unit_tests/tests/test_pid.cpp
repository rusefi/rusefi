/*
 * @file test_pid_auto.cpp
 *
 * @date Sep 29, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

// see also idle.timingPid test

#include "pch.h"

#include "efi_pid.h"

TEST(util, pidScheduledPLeavesCalibrationAndOtherTermsIntact) {
	pid_s params{};
	params.pFactor = 99;
	params.iFactor = 2;
	params.dFactor = 0.1f;
	params.offset = 5;
	params.minValue = -100;
	params.maxValue = 100;
	Pid pid(&params);
	// Error 2: P=6, I=0.4, D=2, offset=5.
	EXPECT_NEAR(pid.getUnclampedOutputWithPFactor(12, 10, 0.1f, 3), 13.4f, 0.0001f);
	EXPECT_FLOAT_EQ(params.pFactor, 99);
	// A different scheduled P keeps accumulated I and the previous error for D.
	EXPECT_NEAR(pid.getUnclampedOutputWithPFactor(12, 10, 0.1f, 4), 13.8f, 0.0001f);
	pid.reset();
	// The ordinary entry point continues to use the calibrated P factor.
	EXPECT_NEAR(pid.getUnclampedOutput(12, 10, 0.1f), 205.4f, 0.0001f);
}

TEST(util, pid) {
	pid_s pidS;
	pidS.pFactor = 50;
	pidS.iFactor = 0.5;
	pidS.dFactor = 0;
	pidS.offset = 0;
	pidS.minValue = 10;
	pidS.maxValue = 90;
	pidS.periodMs = 1;

	Pid pid(&pidS);

	ASSERT_FLOAT_EQ( 90,  pid.getOutput(14, 12, 0.1)) << "getValue#90";


	ASSERT_FLOAT_EQ( 10,  pid.getOutput(14, 16, 0.1)) << "getValue#10";
	ASSERT_FLOAT_EQ(10, pid.getOutput(14, 16, 1));

	pid.updateFactors(29, 0, 0);
	ASSERT_FLOAT_EQ(10, pid.getOutput(14, 16, 1));
//	ASSERT_FLOAT_EQ(68, pid.getIntegration());

	ASSERT_FLOAT_EQ(10, pid.getOutput(14, 16, 1));
//	ASSERT_FLOAT_EQ(0, pid.getIntegration());

	ASSERT_FLOAT_EQ(10, pid.getOutput(14, 16, 1));
//	ASSERT_FLOAT_EQ(68, pid.getIntegration());



	pidS.pFactor = 1;
	pidS.iFactor = 0;
	pidS.dFactor = 0;
	pidS.offset = 0;
	pidS.minValue = 0;
	pidS.maxValue = 100;
	pidS.periodMs = 1;

	pid.reset();

	ASSERT_FLOAT_EQ( 50,  pid.getOutput(/*target*/50, /*input*/0)) << "target=50, input=0";
	ASSERT_FLOAT_EQ( 0,  pid.iTerm) << "target=50, input=0 iTerm";

	ASSERT_FLOAT_EQ( 0,  pid.getOutput(/*target*/50, /*input*/70)) << "target=50, input=70";
	ASSERT_FLOAT_EQ( 0,  pid.iTerm) << "target=50, input=70 iTerm";

	ASSERT_FLOAT_EQ( 0,  pid.getOutput(/*target*/50, /*input*/70)) << "target=50, input=70 #2";
	ASSERT_FLOAT_EQ( 0,  pid.iTerm) << "target=50, input=70 iTerm #2";

	ASSERT_FLOAT_EQ( 0,  pid.getOutput(/*target*/50, /*input*/50)) << "target=50, input=50";
	ASSERT_FLOAT_EQ( 0,  pid.iTerm) << "target=50, input=50 iTerm";
}

static void commonPidTestParameters(pid_s * pidS) {
	pidS->pFactor = 0;
	pidS->iFactor = 50;
	pidS->dFactor = 0;
	pidS->offset = 0;
	pidS->minValue = 10;
	pidS->maxValue = 40;
	pidS->periodMs = 1;
}

static void commonPidTest(Pid *pid) {
	pid->iTermMax = 45;

	ASSERT_FLOAT_EQ( 12.5,  pid->getOutput(/*target*/50, /*input*/0)) << "target=50, input=0 #0";
	ASSERT_FLOAT_EQ( 12.5,  pid->getIntegration());
	ASSERT_FLOAT_EQ( 25  ,  pid->getOutput(/*target*/50, /*input*/0)) << "target=50, input=0 #1";

	ASSERT_FLOAT_EQ( 37.5,  pid->getOutput(/*target*/50, /*input*/0)) << "target=50, input=0 #2";
	ASSERT_FLOAT_EQ( 37.5,  pid->getIntegration());

	ASSERT_FLOAT_EQ( 40.0,  pid->getOutput(/*target*/50, /*input*/0)) << "target=50, input=0 #3";
	ASSERT_FLOAT_EQ( 45,    pid->getIntegration());
}

TEST(util, parallelPidLimits) {
	pid_s pidS;
	commonPidTestParameters(&pidS);

	Pid pid(&pidS);
	commonPidTest(&pid);
}

// [tag:pid_conditional_integration] The opt-in policy changes only clamped
// parallel PID integration. Legacy callers must retain their existing behavior.
TEST(util, pidConditionalIntegrationOptIn) {
	pid_s params{};
	params.pFactor = 1.2f;
	params.iFactor = 1.4f;
	params.minValue = -15;
	params.maxValue = 15;
	Pid legacy(&params);
	Pid limited(&params);
	limited.conditionalIntegration = true;
	for (int i = 0; i < 100; i++) {
		EXPECT_FLOAT_EQ(legacy.getOutput(120, 96, 0.05f), 15);
		EXPECT_FLOAT_EQ(limited.getOutput(120, 96, 0.05f), 15);
	}
	EXPECT_GT(legacy.iTerm, 150);
	EXPECT_NEAR(limited.iTerm, 0, 0.0001f);
	EXPECT_LT(limited.getOutput(120, 125, 0.05f), 0);
	EXPECT_FLOAT_EQ(legacy.getOutput(120, 125, 0.05f), 15);

	limited.reset();
	for (int i = 0; i < 100; i++) {
		EXPECT_FLOAT_EQ(limited.getOutput(96, 120, 0.05f), -15);
	}
	EXPECT_NEAR(limited.iTerm, 0, 0.0001f);
	EXPECT_GT(limited.getOutput(120, 115, 0.05f), 0);
}

TEST(util, pidConditionalIntegrationTracksPdOffsetAndPartialStep) {
	for (float sign : {-1.0f, 1.0f}) {
		SCOPED_TRACE(sign);
		pid_s params{};
		params.pFactor = 1;
		params.iFactor = 2;
		params.dFactor = 0.5f;
		params.offset = sign;
		params.minValue = -10;
		params.maxValue = 10;
		Pid pid(&params);
		pid.conditionalIntegration = true;
		EXPECT_FLOAT_EQ(pid.getOutput(sign * 2, 0, 0.5f), sign * 7);
		EXPECT_FLOAT_EQ(pid.iTerm, sign * 2);
		EXPECT_FLOAT_EQ(pid.getOutput(sign * 2, 0, 0.5f), sign * 7);
		EXPECT_FLOAT_EQ(pid.iTerm, sign * 4);
		// Candidate I=7 would produce output=12. Admit only I=5 so that
		// P=3, D=1 and offset=1 still produce exactly the output limit.
		EXPECT_FLOAT_EQ(pid.getOutput(sign * 3, 0, 0.5f), sign * 10);
		EXPECT_FLOAT_EQ(pid.iTerm, sign * 5);
		EXPECT_FLOAT_EQ(pid.dTerm, sign);
		EXPECT_FLOAT_EQ(pid.previousError, sign * 3);
		EXPECT_FLOAT_EQ(pid.output, sign * 10);
		EXPECT_FLOAT_EQ(pid.target, sign * 3);
		EXPECT_FLOAT_EQ(pid.input, 0);
	}
}

TEST(util, pidConditionalIntegrationUnwindsWhileStillSaturated) {
	pid_s params{};
	params.iFactor = 1;
	params.minValue = -10;
	params.maxValue = 10;
	Pid pid(&params);
	pid.conditionalIntegration = true;
	// Integral could predate opt-in or a change to output limits.
	pid.iTerm = 20;
	EXPECT_FLOAT_EQ(pid.getOutput(0, 2, 0.5f), 10);
	EXPECT_FLOAT_EQ(pid.iTerm, 19);
	pid.iTerm = -20;
	EXPECT_FLOAT_EQ(pid.getOutput(2, 0, 0.5f), -10);
	EXPECT_FLOAT_EQ(pid.iTerm, -19);
}

TEST(util, pidConditionalIntegrationReachesLimitWithIntegralOnly) {
	pid_s params{};
	params.iFactor = 1;
	params.minValue = -10;
	params.maxValue = 10;
	Pid pid(&params);
	pid.conditionalIntegration = true;
	EXPECT_FLOAT_EQ(pid.getOutput(12, 0, 1), 10);
	EXPECT_FLOAT_EQ(pid.iTerm, 10);
	EXPECT_FLOAT_EQ(pid.getOutput(12, 0, 1), 10);
	EXPECT_FLOAT_EQ(pid.iTerm, 10);
	pid.reset();
	EXPECT_FLOAT_EQ(pid.getOutput(0, 12, 1), -10);
	EXPECT_FLOAT_EQ(pid.iTerm, -10);
}

TEST(util, pidConditionalIntegrationRespectsChangedIntegralBounds) {
	pid_s params{};
	params.pFactor = 2;
	params.iFactor = 1;
	params.minValue = -10;
	params.maxValue = 10;
	Pid pid(&params);
	pid.conditionalIntegration = true;
	pid.iTerm = -20;
	pid.iTermMin = -2;
	pid.iTermMax = 2;
	EXPECT_FLOAT_EQ(pid.getOutput(20, 0, 1), 10);
	EXPECT_FLOAT_EQ(pid.iTerm, -2);
	pid.iTerm = 20;
	EXPECT_FLOAT_EQ(pid.getOutput(0, 20, 1), -10);
	EXPECT_FLOAT_EQ(pid.iTerm, 2);
	// Inside output limits the ordinary integral bounds still apply.
	params.pFactor = 0;
	pid.reset();
	EXPECT_FLOAT_EQ(pid.getOutput(5, 0, 1), 2);
	EXPECT_FLOAT_EQ(pid.iTerm, 2);
}

TEST(util, industrialPidLimits) {
	pid_s pidS;
	commonPidTestParameters(&pidS);

	PidIndustrial pid(&pidS);
	commonPidTest(&pid);
}

TEST(util, pidIndustrial) {
	pid_s pidS;
	pidS.pFactor = 1.0;
	pidS.iFactor = 1.0;
	pidS.dFactor = 1.0;
	pidS.offset = 0;
	pidS.minValue = 0;
	pidS.maxValue = 100;
	pidS.periodMs = 1;

	PidIndustrial pid;
	pid.initPidClass(&pidS);

	// we want to compare with the "normal" PID controller
	Pid pid0(&pidS);

	// no additional features
	pid.derivativeFilterLoss = 0;
	pid.antiwindupFreq = 0;

	float industValue = pid.getOutput(/*target*/1, /*input*/0);
	// check if the first output is clamped because of large deviative
	ASSERT_FLOAT_EQ(100.0, industValue);

	// check if all output of the 'zeroed' PidIndustrial (w/o new features) is the same as our "normal" Pid
	for (int i = 0; i < 10; i++) {
		float normalValue = pid0.getOutput(1, 0);
		ASSERT_FLOAT_EQ(normalValue, industValue) << "[" << i << "]";
		industValue = pid.getOutput(1, 0);
	}

	pid.reset();

	// now test the "derivative filter loss" param (some small value)
	pid.derivativeFilterLoss = 0.01;

	// now the first value is less (and not clipped!) due to the derivative filtering
	ASSERT_FLOAT_EQ(67.671669f, pid.getOutput(1, 0));
	// here we still have some leftovers of the initial D-term
	ASSERT_FLOAT_EQ(45.4544487f, pid.getOutput(1, 0));
	// but the value is quickly fading
	ASSERT_FLOAT_EQ(30.6446342f, pid.getOutput(1, 0));

	pid.reset();

	// now test much stronger "derivative filter loss"
	pid.derivativeFilterLoss = 0.1;

	// now the first value is much less due to the derivative filtering
	ASSERT_NEAR(10.5288095f, pid.getOutput(1, 0), EPS4D);
	// here we still have some leftovers of the initial D-term
	ASSERT_NEAR(10.0802946f, pid.getOutput(1, 0), EPS4D);
	// but the fading is slower than with 'weaker' derivative filter above
	ASSERT_NEAR(9.65337563f, pid.getOutput(1, 0), EPS4D);

	pid.reset();
	pid.derivativeFilterLoss = 0;

	// now test "anti-windup" param
	pid.antiwindupFreq = 0.1;

	// the first value is clipped, and that's when the anti-windup comes into effect
	ASSERT_FLOAT_EQ(100.0f, pid.getOutput(1, 0));
	// it stores a small negative offset in the I-term to avoid it's saturation!
	ASSERT_NEAR(-0.0455025025f, pid.getIntegration(), EPS4D);
	// and that's why the second output is smaller then that of normal PID (=1.00999999)
	ASSERT_NEAR(0.959497511f, pid.getOutput(1, 0), EPS4D);

}
