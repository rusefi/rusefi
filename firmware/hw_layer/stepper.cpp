/**
 * @file	stepper.cpp
 *
 * http://rusefi.com/wiki/index.php?title=Hardware:Stepper_motor
 *
 * @date Dec 24, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "stepper.h"
#include "pin_repository.h"

#define ST_DELAY_MS 10

#define ST_COUNT 100

static msg_t stThread(StepperMotor *motor) {
	chRegSetThreadName("stepper");

	palWritePad(motor->directionPort, motor->directionPin, false);

	// let's park the motor in a known position to begin with
	for (int i = 0; i < ST_COUNT; i++) {
		motor->pulse();
	}

	while (true) {
		int targetPosition = motor->targetPosition;
		int currentPosition = motor->currentPosition;

		if (targetPosition == currentPosition) {
			chThdSleepMilliseconds(ST_DELAY_MS);
			continue;
		}
		bool_t isIncrementing = targetPosition > currentPosition;
		palWritePad(motor->directionPort, motor->directionPin, isIncrementing);
		if (isIncrementing) {
			motor->currentPosition++;
		} else {
			motor->currentPosition--;
		}
		motor->pulse();
	}

	// let's part the motor in a known position to begin with
//	for (int i = 0; i < ST_COUNT / 2; i++) {
//		motor->pulse();
//	}

	return 0;
}

StepperMotor::StepperMotor() {
	currentPosition = 0;
	targetPosition = 0;
	directionPort = NULL;
	directionPin = 0;
	stepPort = NULL;
	stepPin = 0;
}

void StepperMotor::pulse() {
	palWritePad(stepPort, stepPin, true);
	chThdSleepMilliseconds(ST_DELAY_MS);
	palWritePad(stepPort, stepPin, false);
	chThdSleepMilliseconds(ST_DELAY_MS);
}

void StepperMotor::initialize(brain_pin_e stepPin, brain_pin_e directionPin) {
	if (stepPin == GPIO_UNASSIGNED || directionPin == GPIO_UNASSIGNED) {
		return;
	}

	stepPort = getHwPort(stepPin);
	this->stepPin = getHwPin(stepPin);

	directionPort = getHwPort(directionPin);
	this->directionPin = getHwPin(directionPin);

	mySetPadMode2("st step", stepPin, PAL_MODE_OUTPUT_PUSHPULL);
	mySetPadMode2("st dir", directionPin, PAL_MODE_OUTPUT_PUSHPULL);

	chThdCreateStatic(stThreadStack, sizeof(stThreadStack), NORMALPRIO, (tfunc_t) stThread, this);
}

