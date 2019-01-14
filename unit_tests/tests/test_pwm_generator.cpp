/*
 * test_pwm_generator.cpp
 *
 *  @date Dec 8, 2018
 *      Author: user
 */

#include "global.h"
#include "unit_test_framework.h"
#include "pwm_generator_logic.h"

#define LOW_VALUE 0
#define HIGH_VALUE 1

extern int timeNowUs;

static int expectedTimeOfNextEvent;
static int pinValue = -1;

static void testApplyPinState(PwmConfig *state, int stateIndex) {
	pinValue = state->multiWave.getChannelState(/*channelIndex*/0, stateIndex);

	printf("PWM_test: setPinValue=%d @ timeNow=%d\r\n", pinValue, timeNowUs);
}

static void assertNextEvent(const char *msg, int expectedPinState, TestExecutor *executor) {
	printf("PWM_test: Asserting event [%s]\r\n", msg);
	// only one action expected in queue
	assertEqualsM("PWM_test: schedulingQueue size", 1, executor->size());

	// move time to next event timestamp
	timeNowUs = expectedTimeOfNextEvent;

	// execute pending actions and assert that only one action was executed
	assertEqualsM5(msg, " executed", 1, executor->executeAll(timeNowUs), 0);
	assertEqualsM5(msg, " pin state", expectedPinState, pinValue, 0);

	// assert that we have one new action in queue
	assertEqualsM("PWM_test: queue.size", 1, executor->size());
}

static void test100dutyCycle() {
	print("*************************************** test100dutyCycle\r\n");

	expectedTimeOfNextEvent = timeNowUs = 0;
	TestExecutor executor;
	SimplePwm pwm("test PWM1");
	OutputPin pin;

	startSimplePwm(&pwm, "unit_test",
			&executor,
			&pin,
			1000 /* frequency */,
			1.0 /* duty cycle */,
			&testApplyPinState);

	expectedTimeOfNextEvent += 1000;
	assertEqualsM2("1@1000/100", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	assertNextEvent("exec@100", HIGH_VALUE, &executor);

	expectedTimeOfNextEvent += 1000;
	assertNextEvent("exec2@100", HIGH_VALUE, &executor);

	expectedTimeOfNextEvent += 1000;
	assertNextEvent("exec3@100", HIGH_VALUE, &executor);
}

static void testSwitchToNanPeriod() {
	print("*************************************** testSwitchToNanPeriod\r\n");

	expectedTimeOfNextEvent = timeNowUs = 0;
	TestExecutor executor;
	SimplePwm pwm("test PWM1");
	OutputPin pin;

	startSimplePwm(&pwm, "unit_test",
			&executor,
			&pin,
			1000 /* frequency */,
			0.60 /* duty cycle */,
			&testApplyPinState);

	expectedTimeOfNextEvent += 600;
	assertEqualsM2("1@1000/70", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	assertNextEvent("exec@70", LOW_VALUE, &executor);
	assertEqualsM("time1", 600, timeNowUs);

	expectedTimeOfNextEvent += 400;
	assertNextEvent("exec2@70", HIGH_VALUE, &executor);

	pwm.setFrequency(NAN);

	expectedTimeOfNextEvent += 600;
	assertEqualsM2("1@1000/NAN", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);
	assertNextEvent("exec2@NAN", LOW_VALUE, &executor);

	expectedTimeOfNextEvent += MS2US(NAN_FREQUENCY_SLEEP_PERIOD_MS);
	assertEqualsM2("2@1000/NAN", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);
	assertNextEvent("exec3@NAN", LOW_VALUE, &executor);
}

TEST(misc, testPwmGenerator) {
	test100dutyCycle();
	testSwitchToNanPeriod();

	print("*************************************** testPwmGenerator\r\n");

	expectedTimeOfNextEvent = timeNowUs = 0;
	TestExecutor executor;
	SimplePwm pwm("test PWM3");
	OutputPin pin;

	startSimplePwm(&pwm,
			"unit_test",
			&executor,
			&pin,
			1000 /* frequency */,
			0.80 /* duty cycle */,
			&testApplyPinState);


	expectedTimeOfNextEvent += 800;
	assertEqualsM2("1@1000/80", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	assertNextEvent("exec@0", LOW_VALUE, &executor);
	assertEqualsM("time1", 800, timeNowUs);

	expectedTimeOfNextEvent += 200;
	assertEqualsM2("2@1000/80", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	// above we had vanilla duty cycle, now let's handle a special case
	pwm.setSimplePwmDutyCycle(0);
	assertEqualsM2("2@1000/0", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	assertNextEvent("exec@1", LOW_VALUE, &executor);
	assertEqualsM("time2", 1000, timeNowUs);

	expectedTimeOfNextEvent += 1000;
	assertEqualsM2("3@1000/0", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	assertNextEvent("exec@2", LOW_VALUE /* pin value */, &executor);
	assertEqualsM("time3", 2000, timeNowUs);
	expectedTimeOfNextEvent += 1000;
	assertEqualsM2("4@1000/0", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	assertNextEvent("exec@3", LOW_VALUE /* pin value */, &executor);
	assertEqualsM("time4", 3000, timeNowUs);
	expectedTimeOfNextEvent += 1000;
	assertEqualsM2("5@1000/0", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	assertNextEvent("exec@4", LOW_VALUE /* pin value */, &executor);
	expectedTimeOfNextEvent += 1000;
	assertEqualsM2("6@1000/0", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	assertNextEvent("exec@5", LOW_VALUE /* pin value */, &executor);
	expectedTimeOfNextEvent += 1000;
	assertEqualsM("time4", 5000, timeNowUs);
	assertEqualsM2("7@1000/0", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);


	assertNextEvent("exec@6", LOW_VALUE /* pin value */, &executor);
}




