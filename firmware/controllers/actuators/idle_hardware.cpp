/**
 * @file    idle_hardware.cpp
 * @brief   Idle Air Control valve hardware
 *
 * @date November 3, 2020
 *
 * This is just the hardware interface - deciding where to put the valve happens in idle_thread.cpp
 */

#include "pch.h"

#if EFI_IDLE_CONTROL
#include "idle_hardware.h"

#include "electronic_throttle.h"

#include "dc_motors.h"
#if ! EFI_UNIT_TEST
#include "stepper.h"
/* Storing two following structs in CCM memory cause HardFault (at least on F4)
 * This need deep debuging. Until it is moved out of CMM. */
static StepDirectionStepper iacStepperHw /*CCM_OPTIONAL*/;
static DualHBridgeStepper iacHbridgeHw /*CCM_OPTIONAL*/;
StepperMotor iacMotor CCM_OPTIONAL;
#endif /* EFI_UNIT_TEST */

static SimplePwm idleSolenoidOpen("idle open");
static SimplePwm idleSolenoidClose("idle close");

void applyIACposition(percent_t position) {
	/**
	 * currently idle level is an percent value (0-100 range), and PWM takes a float in the 0..1 range
	 * todo: unify?
	 */
	float duty = PERCENT_TO_DUTY(position);

	if (engineConfiguration->idleSolenoidInvertPwm) {
		duty = 1.0f - duty;
	}

#if EFI_ELECTRONIC_THROTTLE_BODY
	setEtbIdlePosition(position);
#endif // EFI_ELECTRONIC_THROTTLE_BODY

#if EFI_UNIT_TEST
	if (false) {
#endif // EFI_UNIT_TEST

#if ! EFI_UNIT_TEST
	if (engineConfiguration->useStepperIdle) {
		iacMotor.setTargetPosition(duty * engineConfiguration->idleStepperTotalSteps);
#endif /* EFI_UNIT_TEST */
	} else {
		// if not spinning or running a bench test, turn off the idle valve(s) to be quieter and save power
#if EFI_SHAFT_POSITION_INPUT
		if (!engine->triggerCentral.engineMovedRecently() && engine->timeToStopIdleTest == 0) {
			idleSolenoidOpen.setSimplePwmDutyCycle(0);
			idleSolenoidClose.setSimplePwmDutyCycle(0);
			return;
		}
#endif // EFI_SHAFT_POSITION_INPUT

		if (!engineConfiguration->isDoubleSolenoidIdle) {
			idleSolenoidOpen.setSimplePwmDutyCycle(duty);
		} else {
			// use 0.01..0.99 range
			float idle_range = 0.98; // move to config?

			float idle_open = 0.01 + idle_range * duty;
			float idle_close = 0.01 + idle_range * (1.0 - duty);

			idleSolenoidOpen.setSimplePwmDutyCycle(idle_open);
			idleSolenoidClose.setSimplePwmDutyCycle(idle_close);
		}
	}
}

#if !EFI_UNIT_TEST

bool isIdleHardwareRestartNeeded() {
	return  isConfigurationChanged(stepperEnablePin) ||
			isConfigurationChanged(stepperEnablePinMode) ||
			isConfigurationChanged(idle.stepperStepPin) ||
			isConfigurationChanged(idle.solenoidFrequency) ||
			isConfigurationChanged(useStepperIdle) ||
			isConfigurationChanged(idle.solenoidPin) ||
			isConfigurationChanged(secondSolenoidPin) ||
			isConfigurationChanged(useRawOutputToDriveIdleStepper) ||
			isConfigurationChanged(stepper_raw_output[0])  ||
			isConfigurationChanged(stepper_raw_output[1])  ||
			isConfigurationChanged(stepper_raw_output[2])  ||
			isConfigurationChanged(stepper_raw_output[3]);
}

bool isIdleMotorBusy() {
	if (!engineConfiguration->useStepperIdle) {
		// todo: check other motor types?
		return false;
	}
	return iacMotor.isBusy();
}

void initIdleHardware() {
	if (engineConfiguration->useStepperIdle) {
		StepperHw* hw;

		if (engineConfiguration->useRawOutputToDriveIdleStepper) {
		  // four Push-Pull outputs to directly drive stepper idle air valve coils
			auto motorA = initDcMotor(engineConfiguration->stepper_raw_output[0],
				engineConfiguration->stepper_raw_output[1], ETB_COUNT + 0);
			auto motorB = initDcMotor(engineConfiguration->stepper_raw_output[2],
				engineConfiguration->stepper_raw_output[3], ETB_COUNT + 1);

			iacHbridgeHw.initialize(
				motorA,
				motorB,
				engineConfiguration->idleStepperReactionTime
			);

			hw = &iacHbridgeHw;
		} else if (engineConfiguration->useHbridgesToDriveIdleStepper) {
			auto motorA = initDcMotor("DC dis-1", engineConfiguration->stepperDcIo[0],
				ETB_COUNT + 0, engineConfiguration->stepper_dc_use_two_wires);
			auto motorB = initDcMotor("DC dis-2", engineConfiguration->stepperDcIo[1],
				ETB_COUNT + 1, engineConfiguration->stepper_dc_use_two_wires);

			iacHbridgeHw.initialize(
				motorA,
				motorB,
				engineConfiguration->idleStepperReactionTime
			);

			hw = &iacHbridgeHw;
		} else {
			// like DRV8825?
			iacStepperHw.initialize(
				engineConfiguration->idle.stepperStepPin,
				engineConfiguration->idle.stepperDirectionPin,
				engineConfiguration->stepperDirectionPinMode,
				engineConfiguration->idleStepperReactionTime,
				engineConfiguration->stepperEnablePin,
				engineConfiguration->stepperEnablePinMode
			);

			hw = &iacStepperHw;
		}

		iacMotor.initialize(hw, engineConfiguration->idleStepperTotalSteps);
	} else if (isBrainPinValid(engineConfiguration->idle.solenoidPin)) {
		// we are here for single or double solenoid idle

		/**
		 * Start PWM for idleValvePin
		 */
		// todo: even for double-solenoid mode we can probably use same single SimplePWM
		startSimplePwm(&idleSolenoidOpen, "Idle Valve Open",
			&engine->scheduler,
			&enginePins.idleSolenoidPin,
			engineConfiguration->idle.solenoidFrequency, PERCENT_TO_DUTY(config->cltIdleCorrTable[0][0]));

		if (engineConfiguration->isDoubleSolenoidIdle) {
			if (!isBrainPinValid(engineConfiguration->secondSolenoidPin)) {
				criticalError("Second idle pin should be configured for double solenoid mode.");
				return;
			}

			startSimplePwm(&idleSolenoidClose, "Idle Valve Close",
				&engine->scheduler,
				&enginePins.secondIdleSolenoidPin,
				engineConfiguration->idle.solenoidFrequency, PERCENT_TO_DUTY(config->cltIdleCorrTable[0][0]));
		}
	}
}

#endif

#endif // EFI_IDLE_HARDWARE
