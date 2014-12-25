/**
 * @file	stepper.h
 *
 * @date Dec 24, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */
#ifndef STEPPER_H_
#define STEPPER_H_

#include "main.h"

class StepperMotor {
public:
	void initialize(brain_pin_e stepPin, brain_pin_e directionPin);
	void pulse();
private:

	GPIO_TypeDef * stepPort;
	ioportmask_t stepPin;
	GPIO_TypeDef * directionPort;
	ioportmask_t directionPin;

	int position;

	THD_WORKING_AREA(stThreadStack, UTILITY_THREAD_STACK_SIZE);

};

#endif /* STEPPER_H_ */
