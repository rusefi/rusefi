/**
 * @file    idle_hardware.cpp
 * @brief   Idle Air Control valve hardware
 *
 * @date November 3, 2020
 * 
 * This is just the hardware interface - deciding where to put the valve happens in idle_thread.cpp
 */

#include "global.h"

#if EFI_IDLE_CONTROL
#include "engine_configuration.h"
#include "idle_hardware.h"

#include "engine.h"
#include "electronic_throttle.h"

#include "pwm_generator_logic.h"
#include "dc_motors.h"
#if ! EFI_UNIT_TEST
#include "stepper.h"
#include "pin_repository.h"
static StepDirectionStepper iacStepperHw;
static DualHBridgeStepper iacHbridgeHw;
StepperMotor iacMotor;
#endif /* EFI_UNIT_TEST */

EXTERN_ENGINE;

static Logging* logger;

/**
 * When the IAC position value change is insignificant (lower than this threshold), leave the poor valve alone
 * todo: why do we have this logic? is this ever useful?
 * See
 */
static percent_t idlePositionSensitivityThreshold = 0.0f;

static SimplePwm idleSolenoidOpen("idle open");
static SimplePwm idleSolenoidClose("idle close");

void applyIACposition(percent_t position DECLARE_ENGINE_PARAMETER_SUFFIX) {
	bool prettyClose = absF(position - engine->engineState.idle.currentIdlePosition) < idlePositionSensitivityThreshold;
	// The threshold is dependent on IAC type (see initIdleHardware())
	if (prettyClose) {
		return; // value is pretty close, let's leave the poor valve alone
	}

	
	/**
	 * currently idle level is an percent value (0-100 range), and PWM takes a float in the 0..1 range
	 * todo: unify?
	 */
	float duty = PERCENT_TO_DUTY(position);

	if (CONFIG(useETBforIdleControl)) {
#if EFI_ELECTRONIC_THROTTLE_BODY
		setEtbIdlePosition(position PASS_ENGINE_PARAMETER_SUFFIX);
#endif // EFI_ELECTRONIC_THROTTLE_BODY
#if ! EFI_UNIT_TEST
	} else if (CONFIG(useStepperIdle)) {
		iacMotor.setTargetPosition(duty * engineConfiguration->idleStepperTotalSteps);
#endif /* EFI_UNIT_TEST */
	} else {
		if (!CONFIG(isDoubleSolenoidIdle)) {
			idleSolenoidOpen.setSimplePwmDutyCycle(duty);
		} else {
			/* use 0.01..0.99 range */
			float idle_range = 0.98; /* move to config? */
			float idle_open, idle_close;

			idle_open = 0.01 + idle_range * duty;
			idle_close = 0.01 + idle_range * (1.0 - duty);

			idleSolenoidOpen.setSimplePwmDutyCycle(idle_open);
			idleSolenoidClose.setSimplePwmDutyCycle(idle_close);
		}
	}
}

#if !EFI_UNIT_TEST
extern efitimeus_t timeToStopIdleTest;

static void applyIdleSolenoidPinState(int stateIndex, PwmConfig *state) /* pwm_gen_callback */ {
	efiAssertVoid(CUSTOM_ERR_6645, stateIndex < PWM_PHASE_MAX_COUNT, "invalid stateIndex");
	efiAssertVoid(CUSTOM_ERR_6646, state->multiChannelStateSequence.waveCount == 1, "invalid idle waveCount");
	OutputPin *output = state->outputPins[0];
	int value = state->multiChannelStateSequence.getChannelState(/*channelIndex*/0, stateIndex);
	if (!value /* always allow turning solenoid off */ ||
			(GET_RPM() != 0 || timeToStopIdleTest != 0) /* do not run solenoid unless engine is spinning or bench testing in progress */
			) {
		output->setValue(value);
	}
}

bool isIdleHardwareRestartNeeded() {
	return  isConfigurationChanged(stepperEnablePin) ||
			isConfigurationChanged(stepperEnablePinMode) ||
			isConfigurationChanged(idle.stepperStepPin) ||
			isConfigurationChanged(idle.solenoidFrequency) ||
			isConfigurationChanged(useStepperIdle) ||
//			isConfigurationChanged() ||
			isConfigurationChanged(useETBforIdleControl) ||
			isConfigurationChanged(idle.solenoidPin) ||
			isConfigurationChanged(secondSolenoidPin);
}

bool isIdleMotorBusy(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (!CONFIG(useStepperIdle)) {
		// todo: check other motor types?
		return false;
	}
	return iacMotor.isBusy();
}

void stopIdleHardware(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if EFI_PROD_CODE
	efiSetPadUnused(activeConfiguration.stepperEnablePin);
	efiSetPadUnused(activeConfiguration.idle.stepperStepPin);
	efiSetPadUnused(activeConfiguration.idle.solenoidPin);
	efiSetPadUnused(activeConfiguration.secondSolenoidPin);
#endif /* EFI_PROD_CODE */
}

void initIdleHardware(Logging* sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX) {
	logger = sharedLogger;

	if (CONFIG(useStepperIdle)) {
		StepperHw* hw;

		if (CONFIG(useHbridges)) {
			auto motorA = initDcMotor(engineConfiguration->stepperDcIo[0], 2, /*useTwoWires*/ true PASS_ENGINE_PARAMETER_SUFFIX);
			auto motorB = initDcMotor(engineConfiguration->stepperDcIo[1], 3, /*useTwoWires*/ true PASS_ENGINE_PARAMETER_SUFFIX);

			if (motorA && motorB) {
				iacHbridgeHw.initialize(
					motorA,
					motorB,
					CONFIG(idleStepperReactionTime)
				);
			}

			hw = &iacHbridgeHw;
		} else {
			iacStepperHw.initialize(
				CONFIG(idle).stepperStepPin,
				CONFIG(idle).stepperDirectionPin,
				CONFIG(stepperDirectionPinMode),
				CONFIG(idleStepperReactionTime),
				CONFIG(stepperEnablePin),
				CONFIG(stepperEnablePinMode)
			);

			hw = &iacStepperHw;
		}

		iacMotor.initialize(hw, CONFIG(idleStepperTotalSteps), logger);

		// This greatly improves PID accuracy for steppers with a small number of steps
		idlePositionSensitivityThreshold = 1.0f / engineConfiguration->idleStepperTotalSteps;
	} else if (engineConfiguration->useETBforIdleControl || CONFIG(idle).solenoidPin == GPIO_UNASSIGNED) {
		// here we do nothing for ETB idle and for no idle
	} else {
		// we are here for single or double solenoid idle

		/**
		 * Start PWM for idleValvePin
		 */
		// todo: even for double-solenoid mode we can probably use same single SimplePWM
		// todo: open question why do we pass 'OutputPin' into 'startSimplePwmExt' if we have custom applyIdleSolenoidPinState listener anyway?
		startSimplePwm(&idleSolenoidOpen, "Idle Valve Open",
			&engine->executor,
			&enginePins.idleSolenoidPin,
			CONFIG(idle).solenoidFrequency, PERCENT_TO_DUTY(CONFIG(manIdlePosition)),
			(pwm_gen_callback*)applyIdleSolenoidPinState);

		if (CONFIG(isDoubleSolenoidIdle)) {
			if (CONFIG(secondSolenoidPin) == GPIO_UNASSIGNED) {
				firmwareError(OBD_PCM_Processor_Fault, "Second idle pin should be configured for double solenoid mode.");
				return;
			}

			startSimplePwm(&idleSolenoidClose, "Idle Valve Close",
				&engine->executor,
				&enginePins.secondIdleSolenoidPin,
				CONFIG(idle).solenoidFrequency, PERCENT_TO_DUTY(CONFIG(manIdlePosition)),
				(pwm_gen_callback*)applyIdleSolenoidPinState);
		}

		idlePositionSensitivityThreshold = 0.0f;
	}
}

#endif

#endif // EFI_IDLE_HARDWARE
