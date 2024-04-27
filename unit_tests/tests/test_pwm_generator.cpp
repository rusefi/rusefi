/*
 * test_pwm_generator.cpp
 *
 *  @date Dec 8, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#define LOW_VALUE 0
#define HIGH_VALUE 1
static int expectedTimeOfNextEvent;

static void assertNextEvent(const char *msg, int expectedPinState, TestExecutor *executor, OutputPin& pin) {
	printf("PWM_test: Asserting event [%s]\r\n", msg);
	// only one action expected in queue
	ASSERT_EQ( 1,  executor->size()) << "PWM_test: schedulingQueue size";

	// move time to next event timestamp
	setTimeNowUs(expectedTimeOfNextEvent);

	// execute pending actions and assert that only one action was executed
	ASSERT_EQ(1, executor->executeAll(getTimeNowUs())) << msg << " executed";
	ASSERT_EQ(expectedPinState, pin.m_currentLogicValue) << msg << " pin state";

	// assert that we have one new action in queue
	ASSERT_EQ(1,  executor->size()) << "PWM_test: queue.size";
}

static void test100dutyCycle() {
	printf("*************************************** test100dutyCycle\r\n");

	expectedTimeOfNextEvent = 0;
	setTimeNowUs(0);

	OutputPin pin;
	SimplePwm pwm("test PWM1");
	TestExecutor executor;

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

	expectedTimeOfNextEvent = 0;
	setTimeNowUs(0);

	OutputPin pin;
	SimplePwm pwm("test PWM1");
	TestExecutor executor;

	startSimplePwm(&pwm, "unit_test",
			&executor,
			&pin,
			1000 /* frequency */,
			0.60 /* duty cycle */);

	expectedTimeOfNextEvent += 600;
	assertEqualsM2("1@1000/70", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	assertNextEvent("exec@70", LOW_VALUE, &executor, pin);
	ASSERT_EQ(600, getTimeNowUs()) << "time1";

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

TEST(PWM, testPwmGenerator) {
	test100dutyCycle();
	testSwitchToNanPeriod();

	expectedTimeOfNextEvent = 0;
	setTimeNowUs(0);

	OutputPin pin;
	SimplePwm pwm("test PWM3");
	TestExecutor executor;

	startSimplePwm(&pwm,
			"unit_test",
			&executor,
			&pin,
			1000 /* frequency */,
			0.80 /* duty cycle */);

	expectedTimeOfNextEvent += 800;
	assertEqualsM2("1@1000/80", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	assertNextEvent("exec@0", LOW_VALUE, &executor, pin);
	ASSERT_EQ(800, getTimeNowUs()) << "time1";

	expectedTimeOfNextEvent += 200;
	assertEqualsM2("2@1000/80", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	// above we had vanilla duty cycle, now let's handle a special case
	pwm.setSimplePwmDutyCycle(0);
	assertEqualsM2("2@1000/0", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	assertNextEvent("exec@1", LOW_VALUE, &executor, pin);
	ASSERT_EQ(1000, getTimeNowUs()) << "time2";

	expectedTimeOfNextEvent += 1000;
	assertEqualsM2("3@1000/0", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	assertNextEvent("exec@2", LOW_VALUE /* pin value */, &executor, pin);
	ASSERT_EQ(2000, getTimeNowUs()) << "time3";
	expectedTimeOfNextEvent += 1000;
	assertEqualsM2("4@1000/0", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	assertNextEvent("exec@3", LOW_VALUE /* pin value */, &executor, pin);
	ASSERT_EQ(3000, getTimeNowUs()) << "time4";
	expectedTimeOfNextEvent += 1000;
	assertEqualsM2("5@1000/0", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	assertNextEvent("exec@4", LOW_VALUE /* pin value */, &executor, pin);
	expectedTimeOfNextEvent += 1000;
	assertEqualsM2("6@1000/0", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	assertNextEvent("exec@5", LOW_VALUE /* pin value */, &executor, pin);
	expectedTimeOfNextEvent += 1000;
	ASSERT_EQ(5000, getTimeNowUs()) << "time4";
	assertEqualsM2("7@1000/0", expectedTimeOfNextEvent, executor.getForUnitTest(0)->momentX, 0);

	assertNextEvent("exec@6", LOW_VALUE /* pin value */, &executor, pin);
}
