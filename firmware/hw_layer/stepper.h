/**
 * @file	stepper.h
 *
 * @date Dec 24, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */
#ifndef STEPPER_H_
#define STEPPER_H_

#include "global.h"
#include "efi_gpio.h"
#include "backup_ram.h"
#include "thread_controller.h"

class StepperMotor final : private ThreadController<UTILITY_THREAD_STACK_SIZE> {
public:
	StepperMotor();

	void initialize(brain_pin_e stepPin, brain_pin_e directionPin, pin_output_mode_e directionPinMode, float reactionTime, int totalSteps,
			brain_pin_e enablePin, pin_output_mode_e enablePinMode, Logging *sharedLogger);

	void pulse();
	void setTargetPosition(int targetPosition);
	int getTargetPosition() const;
	void setDirection(bool isIncrementing);

	OutputPin directionPin, stepPin, enablePin;
	int m_currentPosition = 0;
	bool m_currentDirection = false;
	float m_reactionTime = 0;
	int m_totalSteps = 0;

protected:
	void ThreadTask() override;

private:
	int m_targetPosition = 0;

	pin_output_mode_e directionPinMode, stepPinMode, enablePinMode;
};

#endif /* STEPPER_H_ */
