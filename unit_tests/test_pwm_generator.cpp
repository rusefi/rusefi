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

static int expectedTimeOfNextEvent = 0;
static int pinValue = -1;

static void testApplyPinState(PwmConfig *state, int stateIndex) {
	pinValue = state->multiWave.waves[0].pinStates[stateIndex];

	printf("PWM_test: setPinValue=%d @ timeNow=%d\r\n", pinValue, timeNowUs);
}

static void assertNextEvent(const char *msg, int expectedPinState) {
	printf("PWM_test: Asserting event [%s]\r\n", msg);
	// only one action expected in queue
	assertEqualsM("PWM_test: schedulingQueue size", 1, schedulingQueue.size());

	// move time to next event timestamp
	timeNowUs = expectedTimeOfNextEvent;

	// execute pending actions and assert that only one action was executed
	assertEqualsM5(msg, " executed", 1, schedulingQueue.executeAll(timeNowUs), 0);
	assertEqualsM5(msg, " pin state", expectedPinState, pinValue, 0);

	// assert that we have one new action in queue
	assertEqualsM("PWM_test: queue.size", 1, schedulingQueue.size());
}

void testPwmGenerator() {
	print("*************************************** testPwmGenerator\r\n");

	SimplePwm pwm;

	OutputPin pin;

	schedulingQueue.clear();
	timeNowUs = 0;

	startSimplePwm(&pwm, "unit_test",
			&pin,
			1000 /* frequency */,
			0.80 /* duty cycle */,
			&testApplyPinState);


	expectedTimeOfNextEvent += 800;
	assertEqualsM2("1@1000/80", expectedTimeOfNextEvent, schedulingQueue.getForUnitText(0)->momentX, 0);

	assertNextEvent("exec@0", 0);
	assertEqualsM("time1", 800, timeNowUs);

	expectedTimeOfNextEvent += 200;
	assertEqualsM2("2@1000/80", expectedTimeOfNextEvent, schedulingQueue.getForUnitText(0)->momentX, 0);

	// above we had vanilla duty cycle, now let's handle a special case
	pwm.setSimplePwmDutyCycle(0);
	assertEqualsM2("2@1000/0", expectedTimeOfNextEvent, schedulingQueue.getForUnitText(0)->momentX, 0);

	assertNextEvent("exec@1", 1);
	assertEqualsM("time2", 1000, timeNowUs);

	assertEqualsM2("3@1000/0", expectedTimeOfNextEvent, schedulingQueue.getForUnitText(0)->momentX, 0);

	assertNextEvent("exec@2", 0 /* pin value */);
	assertEqualsM("time3", 1000, timeNowUs);
	expectedTimeOfNextEvent += 1000;
	assertEqualsM2("4@1000/0", expectedTimeOfNextEvent, schedulingQueue.getForUnitText(0)->momentX, 0);

	// todo: this is bad - pin is high with zero duty cycle
	assertNextEvent("exec@3", 1 /* pin value */);
	assertEqualsM("time4", 2000, timeNowUs);
	assertEqualsM2("5@1000/0", expectedTimeOfNextEvent, schedulingQueue.getForUnitText(0)->momentX, 0);

	assertNextEvent("exec@4", 0 /* pin value */);
	expectedTimeOfNextEvent += 1000;
	assertEqualsM2("6@1000/0", expectedTimeOfNextEvent, schedulingQueue.getForUnitText(0)->momentX, 0);

	// todo: this is bad - pin is high with zero duty cycle
	assertNextEvent("exec@5", 1 /* pin value */);
	assertEqualsM2("7@1000/0", expectedTimeOfNextEvent, schedulingQueue.getForUnitText(0)->momentX, 0);


	assertNextEvent("exec@6", 0 /* pin value */);
}




