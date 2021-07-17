/**
 * @file	PwmTester.cpp
 * This is a tool to measure rusEfi PWM generation quality
 *
 * @date Apr 29, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */


#if EFI_PWM_TESTER

#include "pwm_tester.h"
#include "state_requence.h"

static SimplePwm pwmTest[5];

extern OutputPin warningLedPin;

static void startPwmTest(int freq) {
	efiPrintf("running pwm test @%d", freq);

	engine->isRunningPwmTest = true;

	// PD13 pin is initialized elsewhere already
	startSimplePwm(&pwmTest[0], "tester", &warningLedPin, 10, 0.5f);
	/**
	 * See custom_engine.cpp for pinout
	 */
	// currently this is PB9 by default - see CONFIG(injectionPins)
	startSimplePwm(&pwmTest[1], "tester", &enginePins.injectors[0], freq / 1.3333333333, 0.5f);
	// currently this is PE2 by default
	startSimplePwm(&pwmTest[2], "tester", &enginePins.injectors[1], freq / 1000, 0.5f);
	// currently this is PB8 by default
	startSimplePwm(&pwmTest[3], "tester", &enginePins.injectors[2], freq, 0.5);
	// currently this is PB7 by default
	startSimplePwm(&pwmTest[4], "tester", &enginePins.injectors[3], freq / 33.33333333333, 0.5);

}

static scheduling_s ioTest;

static OutputSignal outSignals[8];

static void testCallback(void *arg) {

	/**
	 * 0.1ms from now please squirt for 1.6ms
	 */
	float delayMs = 0.1;
	float durationMs = 1.6;

	efitimeus_t nowUs = getTimeNowUs();

	scheduleOutput(&outSignals[0], nowUs, delayMs, durationMs);
	scheduleOutput(&outSignals[1], nowUs, delayMs, durationMs);
	scheduleOutput(&outSignals[2], nowUs, delayMs, durationMs);
	scheduleOutput(&outSignals[3], nowUs, delayMs, durationMs);

	scheduleOutput(&outSignals[4], nowUs, delayMs, durationMs);
	scheduleOutput(&outSignals[5], nowUs, delayMs, durationMs);
	scheduleOutput(&outSignals[6], nowUs, delayMs, durationMs);
	scheduleOutput(&outSignals[7], nowUs, delayMs, durationMs);

	/**
	 * this would re-schedule another callback in 2ms from now
	 */
	engine->executor.scheduleForLater("test", &ioTest, MS2US(2), testCallback);
}

void initPwmTester(void) {
	initLogging(&logger, "pwm test");
	addConsoleActionI("pwmtest", startPwmTest);
	startPwmTest(1000);

	/**
	 * injector channels #4-#8 are used for individual squirt test
	 */
	// todo: yet, it's some horrible code duplication
	outSignals[0].output = &enginePins.injectors[4];
	outSignals[1].output = &enginePins.injectors[5];
	outSignals[2].output = &enginePins.injectors[6];
	outSignals[3].output = &enginePins.injectors[7];
	outSignals[4].output = &enginePins.injectors[8];
	outSignals[5].output = &enginePins.injectors[9];
	outSignals[6].output = &enginePins.injectors[10];
	outSignals[7].output = &enginePins.injectors[11];

	/**
	 * this would schedule a callback in 2ms from now
	 */
	engine->executor.scheduleForLater("test", &ioTest, MS2US(2), testCallback);
}

#endif
