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
	pidS.period = 1;

	Pid pid(&pidS);

	assertEqualsM("getValue#90", 90, pid.getValue(14, 12, 0.1));


	assertEqualsM("getValue#10", 10, pid.getValue(14, 16, 0.1));
	assertEquals(10, pid.getValue(14, 16, 1));

	pid.updateFactors(29, 0, 0);
	assertEquals(10, pid.getValue(14, 16, 1));
//	assertEquals(68, pid.getIntegration());

	assertEquals(10, pid.getValue(14, 16, 1));
//	assertEquals(0, pid.getIntegration());

	assertEquals(10, pid.getValue(14, 16, 1));
//	assertEquals(68, pid.getIntegration());



	pidS.pFactor = 1;
	pidS.iFactor = 0;
	pidS.dFactor = 0;
	pidS.offset = 0;
	pidS.minValue = 0;
	pidS.maxValue = 100;
	pidS.period = 1;

	pid.reset();

	assertEqualsM("target=50, input=0", 50, pid.getValue(/*target*/50, /*input*/0));
	assertEqualsM("target=50, input=0 iTerm", 0, pid.iTerm);

	assertEqualsM("target=50, input=70", 0, pid.getValue(/*target*/50, /*input*/70));
	assertEqualsM("target=50, input=70 iTerm", 0, pid.iTerm);

	assertEqualsM("target=50, input=70 #2", 0, pid.getValue(/*target*/50, /*input*/70));
	assertEqualsM("target=50, input=70 iTerm #2", 0, pid.iTerm);

	assertEqualsM("target=50, input=50", 0, pid.getValue(/*target*/50, /*input*/50));
	assertEqualsM("target=50, input=50 iTerm", 0, pid.iTerm);

}
