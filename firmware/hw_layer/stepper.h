/**
 * @file	stepper.h
 *
 * @date Dec 24, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */
#ifndef STEPPER_H_
#define STEPPER_H_

#include "main.h"

class StepperMotor {
public:
	StepperMotor();
	void initialize(brain_pin_e stepPin, brain_pin_e directionPin);
	void pulse();
	GPIO_TypeDef * directionPort;
	ioportmask_t directionPin;

	int currentPosition;
	int targetPosition;

private:

	GPIO_TypeDef * stepPort;
	ioportmask_t stepPin;

	THD_WORKING_AREA(stThreadStack, UTILITY_THREAD_STACK_SIZE);

};

#endif /* STEPPER_H_ */
