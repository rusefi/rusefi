/**
 * @file	stepper.cpp
 *
 * http://rusefi.com/wiki/index.php?title=Hardware:Stepper_motor
 *
 * @date Dec 24, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "engine.h"

// todo: EFI_STEPPER macro
#if EFI_PROD_CODE || EFI_SIMULATOR
#include "stepper.h"
#include "pin_repository.h"
#include "tps.h"
#include "engine_controller.h"
#include "adc_inputs.h"

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

static msg_t stThread(StepperMotor *motor) {
	chRegSetThreadName("stepper");

	// try to get saved stepper position (-1 for no data)
	motor->currentPosition = loadStepperPos();

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
	bool forceStepperParking = !isRunning && getTPS(PASS_ENGINE_PARAMETER_SIGNATURE) > STEPPER_PARKING_TPS;
	if (CONFIG(stepperForceParkingEveryRestart))
		forceStepperParking = true;
	scheduleMsg(logger, "Stepper: savedStepperPos=%d forceStepperParking=%d (tps=%.2f)", motor->currentPosition, (forceStepperParking ? 1 : 0), getTPS(PASS_ENGINE_PARAMETER_SIGNATURE));

	if (motor->currentPosition < 0 || forceStepperParking) {
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
		int numParkingSteps = (int)efiRound((1.0f + (float)CONFIG(stepperParkingExtraSteps) / PERCENT_MULT) * motor->totalSteps, 1.0f);
		for (int i = 0; i < numParkingSteps; i++) {
			motor->pulse();
		}

		// set & save zero stepper position after the parking completion
		motor->currentPosition = 0;
		saveStepperPos(motor->currentPosition);
	} else {
		// The initial target position should correspond to the saved stepper position.
		// Idle thread starts later and sets a new target position.
		motor->setTargetPosition(motor->currentPosition);
	}

	while (true) {
		int targetPosition = motor->getTargetPosition();
		int currentPosition = motor->currentPosition;

		if (targetPosition == currentPosition) {
			chThdSleepMilliseconds(motor->reactionTime);
			continue;
		}
		bool isIncrementing = targetPosition > currentPosition;
		motor->setDirection(isIncrementing);
		if (isIncrementing) {
			motor->currentPosition++;
		} else {
			motor->currentPosition--;
		}
		motor->pulse();
		// save position to backup RTC register
#if EFI_PROD_CODE
		saveStepperPos(motor->currentPosition);
#endif
	}

	return 0;
}

StepperMotor::StepperMotor() {
	currentPosition = 0;
	targetPosition = 0;

	reactionTime = 0;
	totalSteps = 0;
}

int StepperMotor::getTargetPosition() const {
	return targetPosition;
}

void StepperMotor::setTargetPosition(int targetPosition) {
	this->targetPosition = targetPosition;
}

void StepperMotor::setDirection(bool isIncrementing) {
	if (isIncrementing != this->currentDirection) {
		// compensate stepper motor inertia
		chThdSleepMilliseconds(reactionTime);
		this->currentDirection = isIncrementing;
	}
		
	directionPin.setValue(isIncrementing);
}

void StepperMotor::pulse() {
	enablePin.setValue(false); // enable stepper

	stepPin.setValue(true);
	chThdSleepMilliseconds(reactionTime);

	stepPin.setValue(false);
	chThdSleepMilliseconds(reactionTime);

	enablePin.setValue(true); // disable stepper
}

void StepperMotor::initialize(brain_pin_e stepPin, brain_pin_e directionPin, pin_output_mode_e directionPinMode,
		float reactionTime, int totalSteps,
		brain_pin_e enablePin, pin_output_mode_e enablePinMode, Logging *sharedLogger) {
	this->reactionTime = maxF(1, reactionTime);
	this->totalSteps = maxI(3, totalSteps);
	
	logger = sharedLogger;

	if (stepPin == GPIO_UNASSIGNED || directionPin == GPIO_UNASSIGNED) {
		return;
	}

	this->directionPinMode = directionPinMode;
	this->directionPin.initPin("stepper dir", directionPin, &this->directionPinMode);

	this->stepPinMode = OM_DEFAULT;	// todo: do we need configurable stepPinMode?
	this->stepPin.initPin("stepper step", stepPin, &this->stepPinMode);

	this->enablePinMode = enablePinMode;
	this->enablePin.initPin("stepper enable", enablePin, &this->enablePinMode);

	// All pins must be 0 for correct hardware startup (e.g. stepper auto-disabling circuit etc.).
	this->enablePin.setValue(true); // disable stepper
	this->stepPin.setValue(false);
	this->directionPin.setValue(false);
	this->currentDirection = false;

	chThdCreateStatic(stThreadStack, sizeof(stThreadStack), NORMALPRIO, (tfunc_t)(void*) stThread, this);
}

#endif
