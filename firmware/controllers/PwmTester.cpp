/**
 * @file	PwmTester.cpp
 * This is a tool to measure rusEfi PWM generation quality
 *
 * @date Apr 29, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "PwmTester.h"
#include "EfiWave.h"
#include "pwm_generator_logic.h"
#include "engine_configuration.h"
#include "pwm_generator.h"

static Logging logger;

static SimplePwm pwmTest[5];

extern board_configuration_s *boardConfiguration;

static void startPwmTest(int freq) {
	scheduleMsg(&logger, "running pwm test @%d", freq);

	// PD13, GPIO_UNASSIGNED because pin is initialized elsewhere already
	startSimplePwm(&pwmTest[0], "tester", LED_WARNING, 10, 0.5f);
	// currently this is PB9 by default - see boardConfiguration->injectionPins
	startSimplePwm(&pwmTest[1], "tester", INJECTOR_1_OUTPUT, freq / 1.3333333333, 0.5f);
	// currently this is PB8 by default
	startSimplePwm(&pwmTest[2], "tester", INJECTOR_2_OUTPUT, freq / 1000, 0.5f);
	// currently this is PE3 by default
	startSimplePwm(&pwmTest[3], "tester", INJECTOR_3_OUTPUT, freq, 0.5);
	// currently this is PE5 by default
	startSimplePwm(&pwmTest[4], "tester", INJECTOR_4_OUTPUT, freq / 33.33333333333, 0.5);
}

void initPwmTester(void) {
	initLogging(&logger, "pwm test");
	addConsoleActionI("pwmtest", startPwmTest);
// un-comment this to start pwm test on start up 	startPwmTest(1000);
}
