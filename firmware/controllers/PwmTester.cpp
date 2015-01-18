/**
 * @file	PwmTester.cpp
 * This is a tool to measure rusEfi PWM generation quality
 *
 * @date Apr 29, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "main.h"

#if EFI_PWM_TESTER

#include "PwmTester.h"
#include "EfiWave.h"
#include "pwm_generator_logic.h"
#include "engine.h"
#include "pwm_generator.h"

static LoggingWithStorage logger;

static SimplePwm pwmTest[5];

extern OutputPin warningPin;
extern engine_pins_s enginePins;

EXTERN_ENGINE;

static void startPwmTest(int freq) {
	scheduleMsg(&logger, "running pwm test @%d", freq);

	engine->isRunningPwmTest = true;

	// PD13 pin is initialized elsewhere already
	startSimplePwm(&pwmTest[0], "tester", &warningPin, 10, 0.5f, applyPinState);
	/**
	 * See custom_engine.cpp for pinout
	 */
	// currently this is PB9 by default - see boardConfiguration->injectionPins
	startSimplePwm(&pwmTest[1], "tester", &enginePins.injectors[0], freq / 1.3333333333, 0.5f, applyPinState);
	// currently this is PE2 by default
	startSimplePwm(&pwmTest[2], "tester", &enginePins.injectors[1], freq / 1000, 0.5f, applyPinState);
	// currently this is PB8 by default
	startSimplePwm(&pwmTest[3], "tester", &enginePins.injectors[2], freq, 0.5, applyPinState);
	// currently this is PB7 by default
	startSimplePwm(&pwmTest[4], "tester", &enginePins.injectors[3], freq / 33.33333333333, 0.5, applyPinState);

}

static scheduling_s ioTest;

static OutputSignal outSignals[4];

static void testCallback(void *arg) {

	/**
	 * 0.1ms from now please squirt for 1.6ms
	 */
	scheduleOutput(&outSignals[0], 0.1, 1.6);
	scheduleOutput(&outSignals[1], 0.1, 1.6);
	scheduleOutput(&outSignals[2], 0.1, 1.6);
	scheduleOutput(&outSignals[3], 0.1, 1.6);

	/**
	 * this would re-schedule another callback in 2ms from now
	 */
	scheduleTask("test", &ioTest, MS2US(2), testCallback, NULL);
}

void initPwmTester(void) {
	initLogging(&logger, "pwm test");
	addConsoleActionI("pwmtest", startPwmTest);
	startPwmTest(1000);

	/**
	 * injector channels #4-#8 are used for individual squirt test
	 */
	outSignals[0].output = &enginePins.injectors[4];
	outSignals[1].output = &enginePins.injectors[5];
	outSignals[2].output = &enginePins.injectors[6];
	outSignals[3].output = &enginePins.injectors[7];

	/**
	 * this would schedule a callback in 2ms from now
	 */
	scheduleTask("test", &ioTest, MS2US(2), testCallback, NULL);
}

#endif
