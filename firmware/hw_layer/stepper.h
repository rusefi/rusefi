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
	virtual bool step(bool positive) = 0;
	void pause() const;

protected:
	void setReactionTime(float ms);

private:
	float m_reactionTime = 5.0f;
};

class StepDirectionStepper final : public StepperHw {
public:
	void initialize(brain_pin_e stepPin, brain_pin_e directionPin, pin_output_mode_e directionPinMode, float reactionTime, brain_pin_e enablePin, pin_output_mode_e enablePinMode);

	bool step(bool positive) override;

private:
	bool pulse();
	void setDirection(bool isIncrementing);

	bool m_currentDirection = false;

	OutputPin directionPin, stepPin, enablePin;
	pin_output_mode_e directionPinMode, stepPinMode, enablePinMode;
};

class DcMotor;

class DualHBridgeStepper final : public StepperHw {
public:
    void initialize(DcMotor* motorPhaseA, DcMotor* motorPhaseB, float reactionTime);

    bool step(bool positive) override;

private:
    DcMotor* m_motorPhaseA = nullptr;
    DcMotor* m_motorPhaseB = nullptr;

    uint8_t m_phase = 0;
};

class StepperMotor final : private ThreadController<UTILITY_THREAD_STACK_SIZE> {
public:
	StepperMotor();

	void initialize(StepperHw *hardware, int totalSteps);

	void setTargetPosition(float targetPosition);
	int getTargetPosition() const;

	bool isBusy() const;

	int m_currentPosition = 0;
	int m_totalSteps = 0;

protected:
	void ThreadTask() override;
	void setInitialPosition(void);

	void saveStepperPos(int pos);
	int loadStepperPos();

	void changeCurrentPosition(bool positive);
	void postCurrentPosition(void);

private:
	StepperHw* m_hw = nullptr;

	float m_targetPosition = 0;
	bool initialPositionSet = false;
	bool m_isBusy = false;
};

