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
#include "engine.h"

EXTERN_ENGINE;

static msg_t stThread(StepperMotor *motor) {
	chRegSetThreadName("stepper");

	motor->directionPin.setValue(false);

	/**
	 * let's park the motor in a known position to begin with
	 *
	 * I believe it's safer to retract the valve for parking - at least on a bench I've seen valves
	 * disassembling themselves while pushing too far out.
	 */
	for (int i = 0; i < motor->totalSteps; i++) {
		motor->pulse();
	}

	while (true) {
		int targetPosition = motor->getTargetPosition();
		int currentPosition = motor->currentPosition;

		if (targetPosition == currentPosition) {
			chThdSleepMilliseconds(boardConfiguration->idleStepperPulseDuration);
			continue;
		}
		bool isIncrementing = targetPosition > currentPosition;
		motor->directionPin.setValue(isIncrementing);
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
	chThdSleepMilliseconds(boardConfiguration->idleStepperPulseDuration);
	palWritePad(stepPort, stepPin, false);
	chThdSleepMilliseconds(boardConfiguration->idleStepperPulseDuration);

	palWritePad(enablePort, enablePin, true); // disable stepper
}

void StepperMotor::initialize(brain_pin_e stepPin, brain_pin_e directionPin, pin_output_mode_e directionPinMode,
		float reactionTime, int totalSteps, brain_pin_e enablePin) {
	this->reactionTime = maxF(1, reactionTime);
	this->totalSteps = maxI(3, totalSteps);
	if (stepPin == GPIO_UNASSIGNED || directionPin == GPIO_UNASSIGNED) {
		return;
	}

	stepPort = getHwPort("step", stepPin);
	this->stepPin = getHwPin("step", stepPin);

	this->directionPinMode = directionPinMode;
	this->directionPin.initPin("stepper dir", directionPin, &this->directionPinMode);

	enablePort = getHwPort("enable", enablePin);
	this->enablePin = getHwPin("enable", enablePin);

	efiSetPadMode("stepper step", stepPin, PAL_MODE_OUTPUT_PUSHPULL);
	efiSetPadMode("stepper enable", enablePin, PAL_MODE_OUTPUT_PUSHPULL);
	palWritePad(this->enablePort, enablePin, true); // disable stepper

	chThdCreateStatic(stThreadStack, sizeof(stThreadStack), NORMALPRIO, (tfunc_t) stThread, this);
}

