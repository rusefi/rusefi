/**
 * @file	stepper.cpp
 *
 * http://rusefi.com/wiki/index.php?title=Hardware:Stepper_motor
 *
 * @date Dec 24, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "stepper.h"
#include "pin_repository.h"
#include "engine.h"
#include "tps.h"
#include "engine_controller.h"
#include "adc_inputs.h"

EXTERN_ENGINE;

static Logging *logger;

static void saveStepperPos(int pos) {
	// use backup-power RTC registers to store the data
	backupRamSave(BACKUP_STEPPER_POS, pos + 1);
}

static int loadStepperPos() {
	return (int)backupRamLoad(BACKUP_STEPPER_POS) - 1;
}

static msg_t stThread(StepperMotor *motor) {
	chRegSetThreadName("stepper");

	// try to get saved stepper position (-1 for no data)
	motor->currentPosition = loadStepperPos();

	// first wait until at least 1 slowADC sampling is complete
	waitForSlowAdc();
	// now check if stepper motor re-initialization is requested - if the throttle pedal is pressed at startup
	bool forceStepperParking = !engine->rpmCalculator.isRunning(PASS_ENGINE_PARAMETER_SIGNATURE) && getTPS(PASS_ENGINE_PARAMETER_SIGNATURE) > STEPPER_PARKING_TPS;
	if (boardConfiguration->stepperForceParkingEveryRestart)
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
		int numParkingSteps = (int)efiRound((1.0f + (float)boardConfiguration->stepperParkingExtraSteps / PERCENT_MULT) * motor->totalSteps, 1.0f);
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
		saveStepperPos(motor->currentPosition);
	}

	// let's part the motor in a known position to begin with
//	for (int i = 0; i < ST_COUNT / 2; i++) {
//		motor->pulse();
//	}

	return 0;
}

StepperMotor::StepperMotor() {
	currentPosition = 0;
	targetPosition = 0;
	enablePort = NULL;
	enablePin = 0;
	stepPort = NULL;
	stepPin = 0;
	reactionTime = 0;
	totalSteps = 0;
}

int StepperMotor::getTargetPosition() {
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
	palWritePad(enablePort, enablePin, false); // ebable stepper

	palWritePad(stepPort, stepPin, true);
	chThdSleepMilliseconds(reactionTime);
	palWritePad(stepPort, stepPin, false);
	chThdSleepMilliseconds(reactionTime);

	palWritePad(enablePort, enablePin, true); // disable stepper
}

void StepperMotor::initialize(brain_pin_e stepPin, brain_pin_e directionPin, pin_output_mode_e directionPinMode,
		float reactionTime, int totalSteps, brain_pin_e enablePin, Logging *sharedLogger) {
	this->reactionTime = maxF(1, reactionTime);
	this->totalSteps = maxI(3, totalSteps);
	
	logger = sharedLogger;

	if (stepPin == GPIO_UNASSIGNED || directionPin == GPIO_UNASSIGNED) {
		return;
	}

	stepPort = getHwPort("step", stepPin);
	this->stepPin = getHwPin("step", stepPin);

	this->directionPinMode = directionPinMode;
	this->directionPin.initPin("stepper dir", directionPin, &this->directionPinMode);

	enablePort = getHwPort("enable", enablePin);
	this->enablePin = getHwPin("enable", enablePin);

	efiSetPadMode("stepper step", stepPin, PAL_MODE_OUTPUT_PUSHPULL);
	efiSetPadMode("stepper enable", enablePin, PAL_MODE_OUTPUT_PUSHPULL);
	palWritePad(this->enablePort, enablePin, true); // disable stepper

	// All pins must be 0 for correct hardware startup (e.g. stepper auto-disabling circuit etc.).
	palWritePad(this->stepPort, this->stepPin, false);
	this->directionPin.setValue(false);
	this->currentDirection = false;

	chThdCreateStatic(stThreadStack, sizeof(stThreadStack), NORMALPRIO, (tfunc_t) stThread, this);
}

