/**
 * @file	stepper.h
 *
 * @date Dec 24, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */
#ifndef STEPPER_H_
#define STEPPER_H_

#include "main.h"

class StepperMotor {
public:
	StepperMotor();
	void initialize(brain_pin_e stepPin, brain_pin_e directionPin, float reactionTime, int totalSteps,
			brain_pin_e enablePin);
	void pulse();
	void setTargetPosition(int targetPosition);
	int getTargetPosition();

	ioportid_t directionPort;
	ioportmask_t directionPin;
	int currentPosition;
	float reactionTime;
	int totalSteps;
private:
	int targetPosition;
	ioportid_t stepPort;
	ioportmask_t stepPin;

	ioportid_t enablePort;
	ioportmask_t enablePin;

	THD_WORKING_AREA(stThreadStack, UTILITY_THREAD_STACK_SIZE);
};

#endif /* STEPPER_H_ */
