/**
 * @file	stepper.h
 *
 * @date Dec 24, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"
#include "efi_gpio.h"
#include "backup_ram.h"
#include "thread_controller.h"

class StepperHw {
public:
	virtual void step(bool positive) = 0;
	void pause() const;

protected:
	void setReactionTime(float ms);

private:
	float m_reactionTime = 5.0f;
};

class StepDirectionStepper final : public StepperHw {
public:
	void initialize(brain_pin_e stepPin, brain_pin_e directionPin, pin_output_mode_e directionPinMode, float reactionTime, brain_pin_e enablePin, pin_output_mode_e enablePinMode);

	void step(bool positive) override;

private:
	void pulse();
	void setDirection(bool isIncrementing);

	bool m_currentDirection = false;

	OutputPin directionPin, stepPin, enablePin;
	pin_output_mode_e directionPinMode, stepPinMode, enablePinMode;
};

class DcMotor;

class DualHBridgeStepper final : public StepperHw {
public:
    void initialize(DcMotor* motorPhaseA, DcMotor* motorPhaseB, float reactionTime);

    void step(bool positive) override;

private:
    DcMotor* m_motorPhaseA;
    DcMotor* m_motorPhaseB;

    uint8_t m_phase = 0;
};

class StepperMotor final : private ThreadController<UTILITY_THREAD_STACK_SIZE> {
public:
	StepperMotor();

	void initialize(StepperHw *hardware, int totalSteps, Logging *sharedLogger);

	void setTargetPosition(int targetPosition);
	int getTargetPosition() const;

	int m_currentPosition = 0;
	int m_totalSteps = 0;

protected:
	void ThreadTask() override;

private:
	StepperHw* m_hw = nullptr;

	int m_targetPosition = 0;
};

