/*
 * @file test_pid_auto.cpp
 *
 * @date Sep 29, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

// see also idle.timingPid test

#include "pch.h"

#include "pid.h"

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
