/*
 * test_pwm_generator.cpp
 *
 *  @date Dec 8, 2018
 *      Author: user
 */

#include "pch.h"

#define LOW_VALUE 0
#define HIGH_VALUE 1

extern int timeNowUs;

static int expectedTimeOfNextEvent;

static void assertNextEvent(const char *msg, int expectedPinState, TestExecutor *executor, OutputPin& pin) {
	printf("PWM_test: Asserting event [%s]\r\n", msg);
	// only one action expected in queue
	ASSERT_EQ( 1,  executor->size()) << "PWM_test: schedulingQueue size";

	// move time to next event timestamp
	timeNowUs = expectedTimeOfNextEvent;

	// execute pending actions and assert that only one action was executed
	ASSERT_NEAR(1, executor->executeAll(timeNowUs), 0) << msg << " executed";
	ASSERT_NEAR(expectedPinState, pin.currentLogicValue, 0) << msg << " pin state";

	// assert that we have one new action in queue
	ASSERT_EQ( 1,  executor->size()) << "PWM_test: queue.size";
}

static void test100dutyCycle() {
	printf("*************************************** test100dutyCycle\r\n");

	expectedTimeOfNextEvent = timeNowUs = 0;
	TestExecutor executor;
	SimplePwm pwm("test PWM1");
	OutputPin pin;

	startSimplePwm(&pwm, "unit_test",
			&executor,
			&pin,
			1000 /* frequency */,
			1.0 /* duty cycle */);

	expectedTimeOfNextEvent += 1000;
	assertEqualsM2("1@1000/100", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	assertNextEvent("exec@100", HIGH_VALUE, &executor, pin);

	expectedTimeOfNextEvent += 1000;
	assertNextEvent("exec2@100", HIGH_VALUE, &executor, pin);

	expectedTimeOfNextEvent += 1000;
	assertNextEvent("exec3@100", HIGH_VALUE, &executor, pin);
}

static void testSwitchToNanPeriod() {
	printf("*************************************** testSwitchToNanPeriod\r\n");

	expectedTimeOfNextEvent = timeNowUs = 0;
	TestExecutor executor;
	SimplePwm pwm("test PWM1");
	OutputPin pin;

	startSimplePwm(&pwm, "unit_test",
			&executor,
			&pin,
			1000 /* frequency */,
			0.60 /* duty cycle */);

	expectedTimeOfNextEvent += 600;
	assertEqualsM2("1@1000/70", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	assertNextEvent("exec@70", LOW_VALUE, &executor, pin);
	ASSERT_EQ( 600,  timeNowUs) << "time1";

	expectedTimeOfNextEvent += 400;
	assertNextEvent("exec2@70", HIGH_VALUE, &executor, pin);

	pwm.setFrequency(NAN);

	expectedTimeOfNextEvent += 600;
	assertEqualsM2("1@1000/NAN", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);
	assertNextEvent("exec2@NAN", LOW_VALUE, &executor, pin);

	expectedTimeOfNextEvent += MS2US(NAN_FREQUENCY_SLEEP_PERIOD_MS);
	assertEqualsM2("2@1000/NAN", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);
	assertNextEvent("exec3@NAN", LOW_VALUE, &executor, pin);
}

TEST(misc, testPwmGenerator) {
	test100dutyCycle();
	testSwitchToNanPeriod();

	expectedTimeOfNextEvent = timeNowUs = 0;
	TestExecutor executor;
	SimplePwm pwm("test PWM3");
	OutputPin pin;

	startSimplePwm(&pwm,
			"unit_test",
			&executor,
			&pin,
			1000 /* frequency */,
			0.80 /* duty cycle */);


	expectedTimeOfNextEvent += 800;
	assertEqualsM2("1@1000/80", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	assertNextEvent("exec@0", LOW_VALUE, &executor, pin);
	ASSERT_EQ( 800,  timeNowUs) << "time1";

	expectedTimeOfNextEvent += 200;
	assertEqualsM2("2@1000/80", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	// above we had vanilla duty cycle, now let's handle a special case
	pwm.setSimplePwmDutyCycle(0);
	assertEqualsM2("2@1000/0", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	assertNextEvent("exec@1", LOW_VALUE, &executor, pin);
	ASSERT_EQ( 1000,  timeNowUs) << "time2";

	expectedTimeOfNextEvent += 1000;
	assertEqualsM2("3@1000/0", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	assertNextEvent("exec@2", LOW_VALUE /* pin value */, &executor, pin);
	ASSERT_EQ( 2000,  timeNowUs) << "time3";
	expectedTimeOfNextEvent += 1000;
	assertEqualsM2("4@1000/0", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	assertNextEvent("exec@3", LOW_VALUE /* pin value */, &executor, pin);
	ASSERT_EQ( 3000,  timeNowUs) << "time4";
	expectedTimeOfNextEvent += 1000;
	assertEqualsM2("5@1000/0", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	assertNextEvent("exec@4", LOW_VALUE /* pin value */, &executor, pin);
	expectedTimeOfNextEvent += 1000;
	assertEqualsM2("6@1000/0", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	assertNextEvent("exec@5", LOW_VALUE /* pin value */, &executor, pin);
	expectedTimeOfNextEvent += 1000;
	ASSERT_EQ( 5000,  timeNowUs) << "time4";
	assertEqualsM2("7@1000/0", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);


	assertNextEvent("exec@6", LOW_VALUE /* pin value */, &executor, pin);
}




