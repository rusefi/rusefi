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

void testApplyPinState(PwmConfig *state, int stateIndex) {

}

void testPwmGenerator() {
	print("*************************************** testPwmGenerator\r\n");

	SimplePwm pwm;

	OutputPin pin;

	//pwm.setFrequency(600);

	schedulingQueue.clear();

	startSimplePwm(&pwm, "unit_test",
			&pin,
			600 /* frequency */,
			0.80 /* duty cycle */,
			&testApplyPinState);

	assertEquals(1, schedulingQueue.size());

}


