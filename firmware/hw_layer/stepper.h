/**
 * @file	stepper.h
 *
 * @date Dec 24, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */
#ifndef STEPPER_H_
#define STEPPER_H_

#include "main.h"
#include "efiGpio.h"
#include "backup_ram.h"

class StepperMotor {
public:
	StepperMotor();
	void initialize(brain_pin_e stepPin, brain_pin_e directionPin, pin_output_mode_e directionPinMode, float reactionTime, int totalSteps,
			brain_pin_e enablePin, Logging *sharedLogger);
	void pulse();
	void setTargetPosition(int targetPosition);
	int getTargetPosition();
	void setDirection(bool isIncrementing);

	OutputPin directionPin;
	int currentPosition;
	bool currentDirection;
	float reactionTime;
	int totalSteps;
private:
	int targetPosition;
	ioportid_t stepPort;
	ioportmask_t stepPin;

	ioportid_t enablePort;
	ioportmask_t enablePin;

	pin_output_mode_e directionPinMode;

	THD_WORKING_AREA(stThreadStack, UTILITY_THREAD_STACK_SIZE);
};

#endif /* STEPPER_H_ */
