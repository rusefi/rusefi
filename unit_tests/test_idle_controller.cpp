/*
 * @file test_idle_controller.cpp
 *
 * @date Oct 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include <stdio.h>

#include "efitime.h"
#include "engine_test_helper.h"
#include "pid.h"


void testPidController(void) {
	print("******************************************* testPidController\r\n");
	pid_s pidS;
	pidS.pFactor = 50;
	pidS.iFactor = 0.5;
	pidS.dFactor = 0;
	pidS.offset = 0;

	Pid pid(&pidS, 10, 90);

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

}
