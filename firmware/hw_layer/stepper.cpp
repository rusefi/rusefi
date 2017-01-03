/**
 * @file	stepper.cpp
 *
 * http://rusefi.com/wiki/index.php?title=Hardware:Stepper_motor
 *
 * @date Dec 24, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "stepper.h"
#include "pin_repository.h"

#define ST_DELAY_MS 10

static msg_t stThread(StepperMotor *motor) {
	chRegSetThreadName("stepper");

	palWritePad(motor->directionPort, motor->directionPin, false);

	// let's park the motor in a known position to begin with
	for (int i = 0; i < motor->totalSteps; i++) {
		motor->pulse();
	}

	while (true) {
		int targetPosition = motor->getTargetPosition();
		int currentPosition = motor->currentPosition;

		if (targetPosition == currentPosition) {
			chThdSleepMilliseconds(ST_DELAY_MS);
			continue;
		}
		bool isIncrementing = targetPosition > currentPosition;
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
	enablePort = NULL;
	enablePin = 0;
	stepPort = NULL;
	stepPin = 0;
	reactionTime = 0;
	totalSteps = 0;
}

int StepperMotor::getTargetPosition() {
	return targetPosition;
}

void StepperMotor::setTargetPosition(int targetPosition) {
	this->targetPosition = targetPosition;
}

void StepperMotor::pulse() {
	palWritePad(enablePort, enablePin, false); // ebable stepper

	palWritePad(stepPort, stepPin, true);
	chThdSleepMilliseconds(ST_DELAY_MS);
	palWritePad(stepPort, stepPin, false);
	chThdSleepMilliseconds(ST_DELAY_MS);

	palWritePad(enablePort, enablePin, true); // disable stepper
}

void StepperMotor::initialize(brain_pin_e stepPin, brain_pin_e directionPin, float reactionTime, int totalSteps,
		brain_pin_e enablePin) {
	this->reactionTime = maxF(1, reactionTime);
	this->totalSteps = maxI(3, totalSteps);
	if (stepPin == GPIO_UNASSIGNED || directionPin == GPIO_UNASSIGNED) {
		return;
	}

	stepPort = getHwPort(stepPin);
	this->stepPin = getHwPin(stepPin);

	directionPort = getHwPort(directionPin);
	this->directionPin = getHwPin(directionPin);

	enablePort = getHwPort(enablePin);
	this->enablePin = getHwPin(enablePin);

	mySetPadMode2("stepper step", stepPin, PAL_MODE_OUTPUT_PUSHPULL);
	mySetPadMode2("stepper dir", directionPin, PAL_MODE_OUTPUT_PUSHPULL);
	mySetPadMode2("stepper enable", enablePin, PAL_MODE_OUTPUT_PUSHPULL);
	palWritePad(this->enablePort, enablePin, true); // disable stepper

	chThdCreateStatic(stThreadStack, sizeof(stThreadStack), NORMALPRIO, (tfunc_t) stThread, this);
}

