/**
 * @file	stepper.cpp
 *
 * http://rusefi.com/wiki/index.php?title=Hardware:Stepper_motor
 *
 * @date Dec 24, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "stepper.h"

float StepperMotorBase::getTargetPosition() const {
	return m_targetPosition;
}

void StepperMotorBase::setTargetPosition(float targetPositionSteps) {
	// When the IAC position value change is insignificant (lower than this threshold), leave the poor valve alone
	// When we get a larger change, actually update the target stepper position
	if (std::abs(m_targetPosition - targetPositionSteps) >= 1) {
		m_targetPosition = targetPositionSteps;
	}
}

void StepperMotorBase::initialize(StepperHw *hardware, int totalSteps) {
	m_totalSteps = maxI(3, totalSteps);

	m_hw = hardware;
}

// todo: EFI_STEPPER macro
#if EFI_PROD_CODE || EFI_SIMULATOR

void StepperMotorBase::saveStepperPos(int pos) {
	// use backup-power RTC registers to store the data
#if EFI_PROD_CODE && EFI_BACKUP_SRAM
	backupRamSave(backup_ram_e::StepperPosition, pos + 1);
#endif
	postCurrentPosition();
}

int StepperMotorBase::loadStepperPos() {
#if EFI_PROD_CODE && EFI_BACKUP_SRAM
	return (int)backupRamLoad(backup_ram_e::StepperPosition) - 1;
#else
	return 0;
#endif
}

void StepperMotorBase::changeCurrentPosition(bool positive) {
	if (positive) {
		m_currentPosition++;
	} else {
		m_currentPosition--;
	}
	postCurrentPosition();
}

void StepperMotorBase::postCurrentPosition() {
	if (engineConfiguration->debugMode == DBG_STEPPER_IDLE_CONTROL) {
#if EFI_TUNER_STUDIO
		engine->outputChannels.debugIntField5 = m_currentPosition;
#endif /* EFI_TUNER_STUDIO */
	}
}

void StepperMotorBase::setInitialPosition() {
	// try to get saved stepper position (-1 for no data)
	m_currentPosition = loadStepperPos();

#if HAL_USE_ADC
	// first wait until at least 1 slowADC sampling is complete
	waitForSlowAdc();
#endif

#if EFI_SHAFT_POSITION_INPUT
	bool isRunning = engine->rpmCalculator.isRunning();
#else
	bool isRunning = false;
#endif /* EFI_SHAFT_POSITION_INPUT */
	// now check if stepper motor re-initialization is requested - if the throttle pedal is pressed at startup
	auto tpsPos = Sensor::getOrZero(SensorType::DriverThrottleIntent);
	bool forceStepperParking = !isRunning && tpsPos > STEPPER_PARKING_TPS;
	if (engineConfiguration->stepperForceParkingEveryRestart)
		forceStepperParking = true;
	efiPrintf("Stepper: savedStepperPos=%d forceStepperParking=%d (tps=%.2f)", m_currentPosition, (forceStepperParking ? 1 : 0), tpsPos);

	if (m_currentPosition < 0 || forceStepperParking) {
		efiPrintf("Stepper: starting parking time=%lums", getTimeNowMs());
		// reset saved value
		saveStepperPos(-1);

		/**
		 * let's park the motor in a known position to begin with
		 *
		 * I believe it's safer to retract the valve for parking - at least on a bench I've seen valves
		 * disassembling themselves while pushing too far out.
		 *
		 * Add extra steps to compensate step skipping by some old motors.
		 */
		int numParkingSteps = (int)efiRound((1.0f + (float)engineConfiguration->stepperParkingExtraSteps / PERCENT_MULT) * m_totalSteps, 1.0f);
		for (int i = 0; i < numParkingSteps; i++) {
			if (!m_hw->step(false)) {
				initialPositionSet = false;
				return;
			}
			changeCurrentPosition(false);
		}

		// set & save zero stepper position after the parking completion
		m_currentPosition = 0;
		saveStepperPos(m_currentPosition);
		// todo: is this a slow operation on the start-up path?
		efiPrintf("Stepper: parking finished time=%lums", getTimeNowMs());
	} else {
		// The initial target position should correspond to the saved stepper position.
		// Idle thread starts later and sets a new target position.
		setTargetPosition(m_currentPosition);
	}

	initialPositionSet = true;
}

void StepperMotorBase::doIteration() {
	int targetPosition = efiRound(getTargetPosition(), 1);
	int currentPosition = m_currentPosition;

	// stepper requires +12V
	if (!isIgnVoltage()) {
	  initialPositionSet = false;
		m_hw->pause();
		return;
	}

	if (!initialPositionSet) {
		setInitialPosition();
		return;
	}

	if (targetPosition == currentPosition) {
		m_hw->sleep();
		m_isBusy = false;
		return;
	}

	m_isBusy = true;

	bool isIncrementing = targetPosition > currentPosition;

	if (m_hw->step(isIncrementing)) {
		changeCurrentPosition(isIncrementing);
	}

	// save position to backup RTC register
#if EFI_PROD_CODE
	saveStepperPos(m_currentPosition);
#endif
}

bool StepperMotorBase::isBusy() const {
	return m_isBusy;
}

void StepDirectionStepper::setDirection(bool isIncrementing) {
	if (isIncrementing != m_currentDirection) {
		// compensate stepper motor inertia
		pause();
		m_currentDirection = isIncrementing;
	}

	directionPin.setValue(isIncrementing);
}

bool StepDirectionStepper::pulse() {
	enablePin.setValue(false); // enable stepper

	stepPin.setValue(true);
	pause();

	stepPin.setValue(false);
	pause();

	enablePin.setValue(true); // disable stepper

	return true;
}

void StepperHw::sleep() {
	pause();
}

void StepperHw::pause(int divisor) const {
	// currently we can't sleep less than 1ms (see #3214)
	chThdSleepMicroseconds(maxI(MS2US(1), (int)(MS2US(m_reactionTime)) / divisor));
}

void StepperHw::setReactionTime(float ms) {
	m_reactionTime = std::max(1.0f, ms);
}

bool StepDirectionStepper::step(bool positive) {
	setDirection(positive);
	return pulse();
}

void StepperMotor::initialize(StepperHw *hardware, int totalSteps) {
	StepperMotorBase::initialize(hardware, totalSteps);

	start();
}

void StepDirectionStepper::initialize(brain_pin_e p_stepPin, brain_pin_e p_directionPin, pin_output_mode_e p_directionPinMode, float reactionTime, brain_pin_e p_enablePin, pin_output_mode_e p_enablePinMode) {
	if (!isBrainPinValid(p_stepPin) || !isBrainPinValid(p_directionPin)) {
		return;
	}

	setReactionTime(reactionTime);

	directionPinMode = p_directionPinMode;
	directionPin.initPin("Stepper DIR", p_directionPin, directionPinMode);

	stepPinMode = OM_DEFAULT;	// todo: do we need configurable stepPinMode?
	stepPin.initPin("Stepper step", p_stepPin, stepPinMode);

	enablePinMode = p_enablePinMode;
	enablePin.initPin("Stepper EN", p_enablePin, enablePinMode);

	// All pins must be 0 for correct hardware startup (e.g. stepper auto-disabling circuit etc.).
	enablePin.setValue(true); // disable stepper
	stepPin.setValue(false);
	directionPin.setValue(false);
	m_currentDirection = false;
}

#endif

#if EFI_UNIT_TEST
void StepperHw::sleep() { }
#endif // EFI_UNIT_TEST
