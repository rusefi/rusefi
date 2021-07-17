/**
 * @file	stepper.cpp
 *
 * http://rusefi.com/wiki/index.php?title=Hardware:Stepper_motor
 *
 * @date Dec 24, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */


// todo: EFI_STEPPER macro
#if EFI_PROD_CODE || EFI_SIMULATOR
#include "stepper.h"
#include "pin_repository.h"
#include "engine_controller.h"
#include "adc_inputs.h"
#include "thread_priority.h"

void StepperMotor::saveStepperPos(int pos) {
	// use backup-power RTC registers to store the data
#if EFI_PROD_CODE
	backupRamSave(BACKUP_STEPPER_POS, pos + 1);
#endif
	postCurrentPosition();
}

int StepperMotor::loadStepperPos() {
#if EFI_PROD_CODE
	return (int)backupRamLoad(BACKUP_STEPPER_POS) - 1;
#else
	return 0;
#endif
}

void StepperMotor::changeCurrentPosition(bool positive) {
	if (positive) {
		m_currentPosition++;
	} else {
		m_currentPosition--;
	}
	postCurrentPosition();
}

void StepperMotor::postCurrentPosition(void) {
	if (engineConfiguration->debugMode == DBG_IDLE_CONTROL) {
#if EFI_TUNER_STUDIO
		tsOutputChannels.debugIntField5 = m_currentPosition;
#endif /* EFI_TUNER_STUDIO */
	}
}

void StepperMotor::setInitialPosition(void) {
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
	auto tpsPos = Sensor::get(SensorType::DriverThrottleIntent).value_or(0);
	bool forceStepperParking = !isRunning && tpsPos > STEPPER_PARKING_TPS;
	if (CONFIG(stepperForceParkingEveryRestart))
		forceStepperParking = true;
	efiPrintf("Stepper: savedStepperPos=%d forceStepperParking=%d (tps=%.2f)", m_currentPosition, (forceStepperParking ? 1 : 0), tpsPos);

	if (m_currentPosition < 0 || forceStepperParking) {
		efiPrintf("Stepper: starting parking...");
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
		int numParkingSteps = (int)efiRound((1.0f + (float)CONFIG(stepperParkingExtraSteps) / PERCENT_MULT) * m_totalSteps, 1.0f);
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
		efiPrintf("Stepper: parking finished!");
	} else {
		// The initial target position should correspond to the saved stepper position.
		// Idle thread starts later and sets a new target position.
		setTargetPosition(m_currentPosition);
	}

	initialPositionSet = true;
}

void StepperMotor::ThreadTask() {
	// Require hardware to be set
	if (!m_hw) {
		return;
	}

	while (true) {
		int targetPosition = getTargetPosition();
		int currentPosition = m_currentPosition;

		// the stepper does not work if the main relay is turned off (it requires +12V)
		if (!engine->isMainRelayEnabled()) {
			m_hw->pause();
			continue;
		}

		if (!initialPositionSet) {
			setInitialPosition();
			continue;
		}

		if (targetPosition == currentPosition) {
			m_hw->pause();
			continue;
		}

		bool isIncrementing = targetPosition > currentPosition;

		if (m_hw->step(isIncrementing)) {
			changeCurrentPosition(isIncrementing);
		}

		// save position to backup RTC register
#if EFI_PROD_CODE
		saveStepperPos(m_currentPosition);
#endif
	}
}

StepperMotor::StepperMotor() : ThreadController("stepper", PRIO_STEPPER) {}

int StepperMotor::getTargetPosition() const {
	return m_targetPosition;
}

void StepperMotor::setTargetPosition(int targetPosition) {
	// we accept a new target position only if the motor is powered from the main relay
	if (engine->isMainRelayEnabled()) {
		m_targetPosition = targetPosition;
	}
}

bool StepperMotor::isBusy() const {
	return m_currentPosition != m_targetPosition;
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
	// we move the motor only of it is powered from the main relay
	if (!engine->isMainRelayEnabled())
		return false;

	enablePin.setValue(false); // enable stepper

	stepPin.setValue(true);
	pause();

	stepPin.setValue(false);
	pause();

	enablePin.setValue(true); // disable stepper

	return true;
}

void StepperHw::pause() const {
	chThdSleepMicroseconds((int)(MS2US(m_reactionTime)));
}

void StepperHw::setReactionTime(float ms) {
	m_reactionTime = maxF(1, ms);
}

bool StepDirectionStepper::step(bool positive) {
	setDirection(positive);
	return pulse();
}

void StepperMotor::initialize(StepperHw *hardware, int totalSteps) {
	m_totalSteps = maxI(3, totalSteps);

	m_hw = hardware;

	Start();
}

void StepDirectionStepper::initialize(brain_pin_e stepPin, brain_pin_e directionPin, pin_output_mode_e directionPinMode, float reactionTime, brain_pin_e enablePin, pin_output_mode_e enablePinMode) {
	if (!isBrainPinValid(stepPin) || !isBrainPinValid(directionPin)) {
		return;
	}

	setReactionTime(reactionTime);

	this->directionPinMode = directionPinMode;
	this->directionPin.initPin("Stepper DIR", directionPin, &this->directionPinMode);

	this->stepPinMode = OM_DEFAULT;	// todo: do we need configurable stepPinMode?
	this->stepPin.initPin("Stepper step", stepPin, &this->stepPinMode);

	this->enablePinMode = enablePinMode;
	this->enablePin.initPin("Stepper EN", enablePin, &this->enablePinMode);

	// All pins must be 0 for correct hardware startup (e.g. stepper auto-disabling circuit etc.).
	this->enablePin.setValue(true); // disable stepper
	this->stepPin.setValue(false);
	this->directionPin.setValue(false);
	m_currentDirection = false;
}

#endif
