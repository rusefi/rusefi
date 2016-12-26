/*
 * @file test_idle_controller.cpp
 *
 * @date Oct 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include <stdio.h>
#include "idle_controller.h"
#include "efitime.h"
#include "engine_test_helper.h"
#include "pid.h"

void idleDebug(const char *msg, percent_t value) {
	printf("%s\r\n", msg);
}

static IdleValveState is;

void testIdleController(void) {
	print("******************************************* testIdleController\r\n");
	EngineTestHelper eth(FORD_INLINE_6_1995);
	EXPAND_EngineTestHelper;

	engineConfiguration->targetIdleRpm = 1200;

	is.init(PASS_ENGINE_PARAMETER_F);

	efitimems_t time = 0;

	assertEqualsM("#1", 60.0, is.getIdle(900, time PASS_ENGINE_PARAMETER));

	time += 2000;
	assertEqualsM("idle#2", 60.5, is.getIdle(900, time PASS_ENGINE_PARAMETER));

	time += 2000;
	assertEqualsM("idke#3", 60.6, is.getIdle(1050, time PASS_ENGINE_PARAMETER));
}

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
