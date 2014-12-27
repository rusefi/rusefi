/**
 * @file	stepper.cpp
 *
 * http://rusefi.com/wiki/index.php?title=Hardware:Stepper_motor
 *
 * @date Dec 24, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "stepper.h"
#include "pin_repository.h"

#define ST_DELAY_MS 200

#define ST_COUNT 200

static msg_t stThread(StepperMotor *motor) {
	chRegSetThreadName("stepper");

	// let's part the motor in a known position to begin with
	for (int i = 0; i < ST_COUNT; i++) {
		motor->pulse();
	}
}

void StepperMotor::pulse() {
	palWritePad(stepPort, stepPin, true);
	chThdSleepMilliseconds(ST_DELAY_MS);
	palWritePad(stepPort, stepPin, false);
	chThdSleepMilliseconds(ST_DELAY_MS);
}

void StepperMotor::initialize(brain_pin_e stepPin, brain_pin_e directionPin) {
	position = 0;

	stepPort = getHwPort(stepPin);
	this->stepPin = getHwPin(stepPin);

	mySetPadMode("st step", stepPort, this->stepPin, PAL_MODE_OUTPUT_PUSHPULL);
	mySetPadMode("st dir", getHwPort(directionPin), getHwPin(directionPin), PAL_MODE_OUTPUT_PUSHPULL);

	chThdCreateStatic(stThreadStack, sizeof(stThreadStack), NORMALPRIO, (tfunc_t) stThread, this);
}
