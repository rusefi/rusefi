/*
 * @file test_idle_controller.cpp
 *
 * @date Oct 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "engine_test_helper.h"
#include "pid.h"

TEST(idle, pid) {
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

	ASSERT_EQ( 90,  pid.getValue(14, 12, 0.1)) << "getValue#90";


	ASSERT_EQ( 10,  pid.getValue(14, 16, 0.1)) << "getValue#10";
	ASSERT_EQ(10, pid.getValue(14, 16, 1));

	pid.updateFactors(29, 0, 0);
	ASSERT_EQ(10, pid.getValue(14, 16, 1));
//	ASSERT_EQ(68, pid.getIntegration());

	ASSERT_EQ(10, pid.getValue(14, 16, 1));
//	ASSERT_EQ(0, pid.getIntegration());

	ASSERT_EQ(10, pid.getValue(14, 16, 1));
//	ASSERT_EQ(68, pid.getIntegration());



	pidS.pFactor = 1;
	pidS.iFactor = 0;
	pidS.dFactor = 0;
	pidS.offset = 0;
	pidS.minValue = 0;
	pidS.maxValue = 100;
	pidS.periodMs = 1;

	pid.reset();

	ASSERT_EQ( 50,  pid.getValue(/*target*/50, /*input*/0)) << "target=50, input=0";
	ASSERT_EQ( 0,  pid.iTerm) << "target=50, input=0 iTerm";

	ASSERT_EQ( 0,  pid.getValue(/*target*/50, /*input*/70)) << "target=50, input=70";
	ASSERT_EQ( 0,  pid.iTerm) << "target=50, input=70 iTerm";

	ASSERT_EQ( 0,  pid.getValue(/*target*/50, /*input*/70)) << "target=50, input=70 #2";
	ASSERT_EQ( 0,  pid.iTerm) << "target=50, input=70 iTerm #2";

	ASSERT_EQ( 0,  pid.getValue(/*target*/50, /*input*/50)) << "target=50, input=50";
	ASSERT_EQ( 0,  pid.iTerm) << "target=50, input=50 iTerm";

}
