/*
 * @file test_pid_auto.cpp
 *
 * @date Sep 29, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

// see also idle.timingPid test

#include "unit_test_framework.h"
#include "pid.h"

TEST(util, pid) {
	print("******************************************* testPidController\r\n");
	pid_s pidS;
	pidS.pFactor = 50;
	pidS.iFactor = 0.5;
	pidS.dFactor = 0;
	pidS.offset = 0;
	pidS.minValue = 10;
	pidS.maxValue = 90;
	pidS.periodMs = 1;

	Pid pid(&pidS);

	ASSERT_EQ( 90,  pid.getOutput(14, 12, 0.1)) << "getValue#90";


	ASSERT_EQ( 10,  pid.getOutput(14, 16, 0.1)) << "getValue#10";
	ASSERT_EQ(10, pid.getOutput(14, 16, 1));

	pid.updateFactors(29, 0, 0);
	ASSERT_EQ(10, pid.getOutput(14, 16, 1));
//	ASSERT_EQ(68, pid.getIntegration());

	ASSERT_EQ(10, pid.getOutput(14, 16, 1));
//	ASSERT_EQ(0, pid.getIntegration());

	ASSERT_EQ(10, pid.getOutput(14, 16, 1));
//	ASSERT_EQ(68, pid.getIntegration());



	pidS.pFactor = 1;
	pidS.iFactor = 0;
	pidS.dFactor = 0;
	pidS.offset = 0;
	pidS.minValue = 0;
	pidS.maxValue = 100;
	pidS.periodMs = 1;

	pid.reset();

	ASSERT_EQ( 50,  pid.getOutput(/*target*/50, /*input*/0)) << "target=50, input=0";
	ASSERT_EQ( 0,  pid.iTerm) << "target=50, input=0 iTerm";

	ASSERT_EQ( 0,  pid.getOutput(/*target*/50, /*input*/70)) << "target=50, input=70";
	ASSERT_EQ( 0,  pid.iTerm) << "target=50, input=70 iTerm";

	ASSERT_EQ( 0,  pid.getOutput(/*target*/50, /*input*/70)) << "target=50, input=70 #2";
	ASSERT_EQ( 0,  pid.iTerm) << "target=50, input=70 iTerm #2";

	ASSERT_EQ( 0,  pid.getOutput(/*target*/50, /*input*/50)) << "target=50, input=50";
	ASSERT_EQ( 0,  pid.iTerm) << "target=50, input=50 iTerm";

}

TEST(util, pidLimits) {

	pid_s pidS;
	pidS.pFactor = 0;
	pidS.iFactor = 50;
	pidS.dFactor = 0;
	pidS.offset = 0;
	pidS.minValue = 10;
	pidS.maxValue = 40;
	pidS.periodMs = 1;

	Pid pid(&pidS);

	pid.iTermMax = 45;

	ASSERT_EQ( 12.5,  pid.getOutput(/*target*/50, /*input*/0)) << "target=50, input=0 #0";
	ASSERT_EQ( 25  ,  pid.getOutput(/*target*/50, /*input*/0)) << "target=50, input=0 #1";

	ASSERT_EQ( 37.5,  pid.getOutput(/*target*/50, /*input*/0)) << "target=50, input=0 #2";

	ASSERT_EQ( 40.0,  pid.getOutput(/*target*/50, /*input*/0)) << "target=50, input=0 #3";

}
