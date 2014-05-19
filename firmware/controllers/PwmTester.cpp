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

static float _switchTimes[2];

// todo: extract helper for simple PWM?
static int pinStates[2];
static single_wave_s wave(pinStates);
static single_wave_s sr[1] = { wave };

static PwmConfig pwmTest[5] = { PwmConfig(_switchTimes, sr),
		PwmConfig(_switchTimes, sr),
		PwmConfig(_switchTimes, sr),
		PwmConfig(_switchTimes, sr),
		PwmConfig(_switchTimes, sr)};

extern board_configuration_s *boardConfiguration;

static void startPwmTest(int freq) {
	scheduleMsg(&logger, "running pwm test @%d", freq);

	// PD13, GPIO_NONE because pin is initialized elsewhere already
	startSimplePwm(&pwmTest[0], "tester", GPIO_NONE,
			LED_CRANKING, 0.5, 10, FALSE);
	// currently this is PB9 by default
	startSimplePwm(&pwmTest[1], "tester", boardConfiguration->injectionPins[0],
			INJECTOR_1_OUTPUT, 0.5, freq / 1.3333333333, FALSE);
	// currently this is PB8 by default
	startSimplePwm(&pwmTest[2], "tester", GPIO_NONE,
			INJECTOR_2_OUTPUT, 0.5, freq / 1000, FALSE);
	// currently this is PE3 by default
	startSimplePwm(&pwmTest[3], "tester", GPIO_NONE,
			INJECTOR_3_OUTPUT, 0.5, freq, FALSE);
	// currently this is PE5 by default
	startSimplePwm(&pwmTest[4], "tester", GPIO_NONE,
			INJECTOR_4_OUTPUT, 0.5, freq / 33.33333333333, FALSE);
}

void initPwmTester(void) {
	initLogging(&logger, "pwm test");
	addConsoleActionI("pwmtest", startPwmTest);
// un-comment this to start pwm test on start up 	startPwmTest(1000);
}
