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

#define LOW_VALUE 0
#define HIGH_VALUE 1

extern EventQueue schedulingQueue;
extern int timeNowUs;

static int expectedTimeOfNextEvent;
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

static void test100dutyCycle() {
	print("*************************************** test100dutyCycle\r\n");

	expectedTimeOfNextEvent = timeNowUs = 0;
	SimplePwm pwm;
	OutputPin pin;
	schedulingQueue.clear();

	startSimplePwm(&pwm, "unit_test",
			&pin,
			1000 /* frequency */,
			1.0 /* duty cycle */,
			&testApplyPinState);

	expectedTimeOfNextEvent += 1000;
	assertEqualsM2("1@1000/100", expectedTimeOfNextEvent, schedulingQueue.getForUnitText(0)->momentX, 0);

	assertNextEvent("exec@100", HIGH_VALUE);

	expectedTimeOfNextEvent += 1000;
	assertNextEvent("exec2@100", HIGH_VALUE);

	expectedTimeOfNextEvent += 1000;
	assertNextEvent("exec3@100", HIGH_VALUE);
}

static void testSwitchToNanPeriod() {
	print("*************************************** testSwitchToNanPeriod\r\n");

	expectedTimeOfNextEvent = timeNowUs = 0;
	SimplePwm pwm;
	OutputPin pin;
	schedulingQueue.clear();

	startSimplePwm(&pwm, "unit_test",
			&pin,
			1000 /* frequency */,
			0.60 /* duty cycle */,
			&testApplyPinState);

	expectedTimeOfNextEvent += 600;
	assertEqualsM2("1@1000/70", expectedTimeOfNextEvent, schedulingQueue.getForUnitText(0)->momentX, 0);

	assertNextEvent("exec@70", LOW_VALUE);
	assertEqualsM("time1", 600, timeNowUs);

	expectedTimeOfNextEvent += 400;
	assertNextEvent("exec2@70", HIGH_VALUE);

	pwm.setFrequency(NAN);

	expectedTimeOfNextEvent += 600;
	assertEqualsM2("1@1000/NAN", expectedTimeOfNextEvent, schedulingQueue.getForUnitText(0)->momentX, 0);
	assertNextEvent("exec2@NAN", LOW_VALUE);

	expectedTimeOfNextEvent += MS2US(NAN_FREQUENCY_SLEEP_PERIOD_MS);
	assertEqualsM2("2@1000/NAN", expectedTimeOfNextEvent, schedulingQueue.getForUnitText(0)->momentX, 0);
	assertNextEvent("exec3@NAN", LOW_VALUE);
}

void testPwmGenerator() {
	test100dutyCycle();
	testSwitchToNanPeriod();

	print("*************************************** testPwmGenerator\r\n");

	expectedTimeOfNextEvent = timeNowUs = 0;
	SimplePwm pwm;
	OutputPin pin;
	schedulingQueue.clear();

	startSimplePwm(&pwm, "unit_test",
			&pin,
			1000 /* frequency */,
			0.80 /* duty cycle */,
			&testApplyPinState);


	expectedTimeOfNextEvent += 800;
	assertEqualsM2("1@1000/80", expectedTimeOfNextEvent, schedulingQueue.getForUnitText(0)->momentX, 0);

	assertNextEvent("exec@0", LOW_VALUE);
	assertEqualsM("time1", 800, timeNowUs);

	expectedTimeOfNextEvent += 200;
	assertEqualsM2("2@1000/80", expectedTimeOfNextEvent, schedulingQueue.getForUnitText(0)->momentX, 0);

	// above we had vanilla duty cycle, now let's handle a special case
	pwm.setSimplePwmDutyCycle(0);
	assertEqualsM2("2@1000/0", expectedTimeOfNextEvent, schedulingQueue.getForUnitText(0)->momentX, 0);

	assertNextEvent("exec@1", LOW_VALUE);
	assertEqualsM("time2", 1000, timeNowUs);

	expectedTimeOfNextEvent += 1000;
	assertEqualsM2("3@1000/0", expectedTimeOfNextEvent, schedulingQueue.getForUnitText(0)->momentX, 0);

	assertNextEvent("exec@2", LOW_VALUE /* pin value */);
	assertEqualsM("time3", 2000, timeNowUs);
	expectedTimeOfNextEvent += 1000;
	assertEqualsM2("4@1000/0", expectedTimeOfNextEvent, schedulingQueue.getForUnitText(0)->momentX, 0);

	assertNextEvent("exec@3", LOW_VALUE /* pin value */);
	assertEqualsM("time4", 3000, timeNowUs);
	expectedTimeOfNextEvent += 1000;
	assertEqualsM2("5@1000/0", expectedTimeOfNextEvent, schedulingQueue.getForUnitText(0)->momentX, 0);

	assertNextEvent("exec@4", LOW_VALUE /* pin value */);
	expectedTimeOfNextEvent += 1000;
	assertEqualsM2("6@1000/0", expectedTimeOfNextEvent, schedulingQueue.getForUnitText(0)->momentX, 0);

	assertNextEvent("exec@5", LOW_VALUE /* pin value */);
	expectedTimeOfNextEvent += 1000;
	assertEqualsM("time4", 5000, timeNowUs);
	assertEqualsM2("7@1000/0", expectedTimeOfNextEvent, schedulingQueue.getForUnitText(0)->momentX, 0);


	assertNextEvent("exec@6", LOW_VALUE /* pin value */);
}




