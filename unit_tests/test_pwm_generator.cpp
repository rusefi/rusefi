/*
 * test_pwm_generator.cpp
 *
 *  @date Dec 8, 2018
 *      Author: user
 */

#include "global.h"
#include "unit_test_framework.h"
#include "event_queue.h"
#include "pwm_generator_logic.h"

extern EventQueue schedulingQueue;
extern int timeNowUs;

static int nextTime = 800;

static void testApplyPinState(PwmConfig *state, int stateIndex) {
	int value = state->multiWave.waves[0].pinStates[stateIndex];

	printf("Applying value %d @ timeNow=%d\r\n", value, timeNowUs);
}

static void assertNextEvent(const char *msg) {
	printf("Asserting event [%s]\r\n", msg);
	// only one action expected in queue
	assertEquals(1, schedulingQueue.size());

	// move time to next event timestamp
	timeNowUs += nextTime;

	// execute pending actions and assert that only one action was executed
	assertEqualsM(msg, 1, schedulingQueue.executeAll(timeNowUs));

	// assert that we have one new action in queue
	assertEquals(1, schedulingQueue.size());
}

void testPwmGenerator() {
	print("*************************************** testPwmGenerator\r\n");

	SimplePwm pwm;

	OutputPin pin;

	schedulingQueue.clear();

	startSimplePwm(&pwm, "unit_test",
			&pin,
			1000 /* frequency */,
			0.80 /* duty cycle */,
			&testApplyPinState);

	int start = timeNowUs;

	assertEqualsM2("1@1000/80", start + nextTime, schedulingQueue.getForUnitText(0)->momentX, 0);

	assertNextEvent("exec@0");
	assertEqualsM("time1", start + 800, timeNowUs);

	nextTime += 200;
	assertEqualsM2("2@1000/80", start + nextTime, schedulingQueue.getForUnitText(0)->momentX, 0);


	pwm.setSimplePwmDutyCycle(0);

	assertNextEvent("exec@1");
	assertEqualsM("time2", start + 1800, timeNowUs);

	assertEqualsM2("2@1000/80", start + nextTime, schedulingQueue.getForUnitText(0)->momentX, 0);

	assertNextEvent("exec@2");
	assertEqualsM("time3", start + 2800, timeNowUs);

	assertNextEvent("exec@3");
	assertEqualsM("time4", start + 3800, timeNowUs);

	assertNextEvent("exec@4");
	assertNextEvent("exec@5");
}




