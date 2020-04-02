/**
 * @file	stepper.cpp
 *
 * http://rusefi.com/wiki/index.php?title=Hardware:Stepper_motor
 *
 * @date Dec 24, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "engine.h"

// todo: EFI_STEPPER macro
#if EFI_PROD_CODE || EFI_SIMULATOR
#include "stepper.h"
#include "pin_repository.h"
#include "engine_controller.h"
#include "adc_inputs.h"
#include "sensor.h"

EXTERN_ENGINE;

static Logging *logger;

static void saveStepperPos(int pos) {
	// use backup-power RTC registers to store the data
#if EFI_PROD_CODE
	backupRamSave(BACKUP_STEPPER_POS, pos + 1);
#endif
}

static int loadStepperPos() {
#if EFI_PROD_CODE
	return (int)backupRamLoad(BACKUP_STEPPER_POS) - 1;
#else
	return 0;
#endif
}

void StepperMotor::ThreadTask() {
	// Require hardware to be set
	if (!m_hw) {
		return;
	}

	// try to get saved stepper position (-1 for no data)
	m_currentPosition = loadStepperPos();

#if HAL_USE_ADC
	// first wait until at least 1 slowADC sampling is complete
	waitForSlowAdc();
#endif
#if EFI_SHAFT_POSITION_INPUT
	bool isRunning = engine->rpmCalculator.isRunning(PASS_ENGINE_PARAMETER_SIGNATURE);
#else
	bool isRunning = false;
#endif /* EFI_SHAFT_POSITION_INPUT */
	// now check if stepper motor re-initialization is requested - if the throttle pedal is pressed at startup
	auto tpsPos = Sensor::get(SensorType::DriverThrottleIntent).value_or(0);
	bool forceStepperParking = !isRunning && tpsPos > STEPPER_PARKING_TPS;
	if (CONFIG(stepperForceParkingEveryRestart))
		forceStepperParking = true;
	scheduleMsg(logger, "Stepper: savedStepperPos=%d forceStepperParking=%d (tps=%.2f)", m_currentPosition, (forceStepperParking ? 1 : 0), tpsPos);

	if (m_currentPosition < 0 || forceStepperParking) {
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
			m_hw->step(false);
		}

		// set & save zero stepper position after the parking completion
		m_currentPosition = 0;
		saveStepperPos(m_currentPosition);
	} else {
		// The initial target position should correspond to the saved stepper position.
		// Idle thread starts later and sets a new target position.
		setTargetPosition(m_currentPosition);
	}

	while (true) {
		int targetPosition = getTargetPosition();
		int currentPosition = m_currentPosition;

		if (targetPosition == currentPosition) {
			m_hw->pause();
			continue;
		}
		bool isIncrementing = targetPosition > currentPosition;

		if (isIncrementing) {
			m_currentPosition++;
		} else {
			m_currentPosition--;
		}

		m_hw->step(isIncrementing);

		// save position to backup RTC register
#if EFI_PROD_CODE
		saveStepperPos(m_currentPosition);
#endif
	}
}

StepperMotor::StepperMotor() : ThreadController("stepper", NORMALPRIO) {}

int StepperMotor::getTargetPosition() const {
	return m_targetPosition;
}

void StepperMotor::setTargetPosition(int targetPosition) {
	m_targetPosition = targetPosition;
}

void StepDirectionStepper::setDirection(bool isIncrementing) {
	if (isIncrementing != m_currentDirection) {
		// compensate stepper motor inertia
		pause();
		m_currentDirection = isIncrementing;
	}

	directionPin.setValue(isIncrementing);
}

void StepDirectionStepper::pulse() {
	enablePin.setValue(false); // enable stepper

	stepPin.setValue(true);
	pause();

	stepPin.setValue(false);
	pause();

	enablePin.setValue(true); // disable stepper
}

void StepperHw::pause() const {
	chThdSleepMilliseconds(m_reactionTime);
}

void StepperHw::setReactionTime(float ms) {
	m_reactionTime = maxF(1, ms);
}

void StepDirectionStepper::step(bool positive) {
	setDirection(positive);
	pulse();
}

void StepperMotor::initialize(StepperHw *hardware, int totalSteps, Logging *sharedLogger) {
	m_totalSteps = maxI(3, totalSteps);

	m_hw = hardware;

	logger = sharedLogger;

	Start();
}

void StepDirectionStepper::initialize(brain_pin_e stepPin, brain_pin_e directionPin, pin_output_mode_e directionPinMode, float reactionTime, brain_pin_e enablePin, pin_output_mode_e enablePinMode) {
	if (stepPin == GPIO_UNASSIGNED || directionPin == GPIO_UNASSIGNED) {
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
