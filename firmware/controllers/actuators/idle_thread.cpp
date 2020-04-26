/**
 * @file    idle_thread.cpp
 * @brief   Idle Air Control valve thread.
 *
 * This thread looks at current RPM and decides if it should increase or decrease IAC duty cycle.
 * This file has the hardware & scheduling logic, desired idle level lives separately.
 *
 *
 * @date May 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 * enable verbose_idle
 * disable verbose_idle
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "global.h"

#if EFI_IDLE_CONTROL
#include "engine_configuration.h"
#include "rpm_calculator.h"
#include "pwm_generator_logic.h"
#include "idle_thread.h"
#include "engine_math.h"

#include "engine.h"
#include "periodic_task.h"
#include "allsensors.h"
#include "sensor.h"
#include "electronic_throttle.h"

#if ! EFI_UNIT_TEST
#include "stepper.h"
#include "dc_motors.h"
#include "pin_repository.h"
static StepDirectionStepper iacStepperHw;
static DualHBridgeStepper iacHbridgeHw;
static StepperMotor iacMotor;
#endif /* EFI_UNIT_TEST */


static Logging *logger;

EXTERN_ENGINE;

static bool shouldResetPid = false;
// The idea of 'mightResetPid' is to reset PID only once - each time when TPS > idlePidDeactivationTpsThreshold.
// The throttle pedal can be pressed for a long time, making the PID data obsolete (thus the reset is required).
// We set 'mightResetPid' to true only if PID was actually used (i.e. idlePid.getOutput() was called) to save some CPU resources.
// See automaticIdleController().
static bool mightResetPid = false;

#if EFI_IDLE_PID_CIC
// Use new PID with CIC integrator
PidCic idlePid;
#else

class PidWithOverrides : public Pid {
public:
	float getOffset() const override {
#if EFI_FSIO && ! EFI_UNIT_TEST
			if (engineConfiguration->useFSIO12ForIdleOffset) {
				return ENGINE(fsioState.fsioIdleOffset);
			}
#endif /* EFI_FSIO */
		return parameters->offset;
	}

	float getMinValue() const override {
#if EFI_FSIO && ! EFI_UNIT_TEST
			if (engineConfiguration->useFSIO13ForIdleMinValue) {
				return ENGINE(fsioState.fsioIdleMinValue);
			}
#endif /* EFI_FSIO */
		return parameters->minValue;
	}
};

PidWithOverrides idlePid;
#endif /* EFI_IDLE_PID_CIC */

// todo: extract interface for idle valve hardware, with solenoid and stepper implementations?
static SimplePwm idleSolenoidOpen("idle open");
static SimplePwm idleSolenoidClose("idle close");

static uint32_t lastCrankingCyclesCounter = 0;
static float lastCrankingIacPosition;

static iacPidMultiplier_t iacPidMultMap("iacPidMultiplier");

/**
 * When the IAC position value change is insignificant (lower than this threshold), leave the poor valve alone
 * todo: why do we have this logic? is this ever useful?
 * See
 */
static percent_t idlePositionSensitivityThreshold = 0.0f;

#if ! EFI_UNIT_TEST

void idleDebug(const char *msg, percent_t value) {
	scheduleMsg(logger, "idle debug: %s%.2f", msg, value);
}

static void showIdleInfo(void) {
	const char * idleModeStr = getIdle_mode_e(engineConfiguration->idleMode);
	scheduleMsg(logger, "idleMode=%s position=%.2f isStepper=%s", idleModeStr,
			getIdlePosition(), boolToString(CONFIG(useStepperIdle)));

	if (CONFIG(useStepperIdle)) {
		scheduleMsg(logger, "directionPin=%s reactionTime=%.2f", hwPortname(CONFIG(idle).stepperDirectionPin),
				engineConfiguration->idleStepperReactionTime);
		scheduleMsg(logger, "stepPin=%s steps=%d", hwPortname(CONFIG(idle).stepperStepPin),
				engineConfiguration->idleStepperTotalSteps);
		scheduleMsg(logger, "enablePin=%s/%d", hwPortname(engineConfiguration->stepperEnablePin),
				engineConfiguration->stepperEnablePinMode);
	} else {
		if (!CONFIG(isDoubleSolenoidIdle)) {
			scheduleMsg(logger, "idle valve freq=%d on %s", CONFIG(idle).solenoidFrequency,
					hwPortname(CONFIG(idle).solenoidPin));
		} else {
			scheduleMsg(logger, "idle valve freq=%d on %s", CONFIG(idle).solenoidFrequency,
					hwPortname(CONFIG(idle).solenoidPin));
			scheduleMsg(logger, " and %s", hwPortname(CONFIG(secondSolenoidPin)));
		}
	}

	if (engineConfiguration->idleMode == IM_AUTO) {
		idlePid.showPidStatus(logger, "idle");
	}
}

void setIdleMode(idle_mode_e value) {
	engineConfiguration->idleMode = value ? IM_AUTO : IM_MANUAL;
	showIdleInfo();
}

static void applyIACposition(percent_t position) {
	/**
	 * currently idle level is an percent value (0-100 range), and PWM takes a float in the 0..1 range
	 * todo: unify?
	 */
	float duty = PERCENT_TO_DUTY(position);

	if (CONFIG(useETBforIdleControl)) {
		if (!Sensor::hasSensor(SensorType::AcceleratorPedal)) {
			firmwareError(CUSTOM_NO_ETB_FOR_IDLE, "No ETB to use for idle");
			return;
		}

#if EFI_ELECTRONIC_THROTTLE_BODY
		setEtbIdlePosition(position);
#endif
#if ! EFI_UNIT_TEST
	} if (CONFIG(useStepperIdle)) {
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

percent_t getIdlePosition(void) {
	return engine->engineState.idle.currentIdlePosition;
}

void setIdleValvePosition(int positionPercent) {
	if (positionPercent < 1 || positionPercent > 99)
		return;
	scheduleMsg(logger, "setting idle valve position %d", positionPercent);
#if ! EFI_UNIT_TEST
	showIdleInfo();
#endif /* EFI_UNIT_TEST */
	// todo: this is not great that we have to write into configuration here
	CONFIG(manIdlePosition) = positionPercent;
}

#endif /* EFI_UNIT_TEST */

static percent_t manualIdleController(float cltCorrection DECLARE_ENGINE_PARAMETER_SUFFIX) {

	percent_t correctedPosition = cltCorrection * CONFIG(manIdlePosition);

	return correctedPosition;
}

/**
 * idle blip is a development tool: alternator PID research for instance have benefited from a repetitive change of RPM
 */
static percent_t blipIdlePosition;
static efitimeus_t timeToStopBlip = 0;
static efitimeus_t timeToStopIdleTest = 0;

/**
 * I use this questionable feature to tune acceleration enrichment
 */
static void blipIdle(int idlePosition, int durationMs) {
	if (timeToStopBlip != 0) {
		return; // already in idle blip
	}
	blipIdlePosition = idlePosition;
	timeToStopBlip = getTimeNowUs() + 1000 * durationMs;
}

static void finishIdleTestIfNeeded() {
	if (timeToStopIdleTest != 0 && getTimeNowUs() > timeToStopIdleTest)
		timeToStopIdleTest = 0;
}

static void undoIdleBlipIfNeeded() {
	if (timeToStopBlip != 0 && getTimeNowUs() > timeToStopBlip) {
		timeToStopBlip = 0;
	}
}

static bool isOutOfAutomaticIdleCondition(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (CONFIG(throttlePedalUpPin) != GPIO_UNASSIGNED) {
		return !engine->engineState.idle.throttlePedalUpState;
	}

	const auto [valid, pos] = Sensor::get(SensorType::DriverThrottleIntent);

	// Disable auto idle in case of TPS/Pedal failure
	if (!valid) {
		return true;
	}

	return pos > CONFIG(idlePidDeactivationTpsThreshold);
}

/**
 * @return idle valve position percentage for automatic closed loop mode
 */
static percent_t automaticIdleController(float tpsPos DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (isOutOfAutomaticIdleCondition(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		// Don't store old I and D terms if PID doesn't work anymore.
		// Otherwise they will affect the idle position much later, when the throttle is closed.
		if (mightResetPid) {
			mightResetPid = false;
			shouldResetPid = true;
		}

		engine->engineState.idle.idleState = TPS_THRESHOLD;
		// just leave IAC position as is (but don't return currentIdlePosition - it may already contain additionalAir)
		return engine->engineState.idle.baseIdlePosition;
	}

	// get Target RPM for Auto-PID from a separate table
	int targetRpm = getTargetRpmForIdleCorrection(PASS_ENGINE_PARAMETER_SIGNATURE);

	efitick_t nowNt = getTimeNowNt();

	float rpm;
	if (CONFIG(useInstantRpmForIdle)) {
		rpm = engine->triggerCentral.triggerState.calculateInstantRpm(NULL, nowNt PASS_ENGINE_PARAMETER_SUFFIX);
	} else {
		rpm = GET_RPM();
	}

	// check if within the dead zone
	if (absI(rpm - targetRpm) <= CONFIG(idlePidRpmDeadZone)) {
		engine->engineState.idle.idleState = RPM_DEAD_ZONE;
		// current RPM is close enough, no need to change anything
		return engine->engineState.idle.baseIdlePosition;
	}

	// When rpm < targetRpm, there's a risk of dropping RPM too low - and the engine dies out.
	// So PID reaction should be increased by adding extra percent to PID-error:
	percent_t errorAmpCoef = 1.0f;
	if (rpm < targetRpm)
		errorAmpCoef += (float)CONFIG(pidExtraForLowRpm) / PERCENT_MULT;
	// If errorAmpCoef > 1.0, then PID thinks that RPM is lower than it is, and controls IAC more aggressively
	idlePid.setErrorAmplification(errorAmpCoef);

	percent_t newValue = idlePid.getOutput(targetRpm, rpm);
	engine->engineState.idle.idleState = PID_VALUE;

	// the state of PID has been changed, so we might reset it now, but only when needed (see idlePidDeactivationTpsThreshold)
	mightResetPid = true;

	// Apply PID Multiplier if used
	if (CONFIG(useIacPidMultTable)) {
		float engineLoad = getEngineLoadT(PASS_ENGINE_PARAMETER_SIGNATURE);
		float multCoef = iacPidMultMap.getValue(rpm / RPM_1_BYTE_PACKING_MULT, engineLoad);
		// PID can be completely disabled of multCoef==0, or it just works as usual if multCoef==1
		newValue = interpolateClamped(0.0f, engine->engineState.idle.baseIdlePosition, 1.0f, newValue, multCoef);
	}
	
	// Apply PID Deactivation Threshold as a smooth taper for TPS transients.
	// if tps==0 then PID just works as usual, or we completely disable it if tps>=threshold
	newValue = interpolateClamped(0.0f, newValue, CONFIG(idlePidDeactivationTpsThreshold), engine->engineState.idle.baseIdlePosition, tpsPos);

	// Interpolate to the manual position when RPM is close to the upper RPM limit (if idlePidRpmUpperLimit is set).
	// If RPM increases and the throttle is closed, then we're in coasting mode, and we should smoothly disable auto-pid.
	// If we just leave IAC at baseIdlePosition (as in case of TPS deactivation threshold), RPM would get stuck. 
	// That's why there's 'useIacTableForCoasting' setting which involves a separate IAC position table for coasting (iacCoasting).
	// Currently it's user-defined. But eventually we'll use a real calculated and stored IAC position instead.
	int idlePidLowerRpm = targetRpm + CONFIG(idlePidRpmDeadZone);
	if (CONFIG(idlePidRpmUpperLimit) > 0) {
		engine->engineState.idle.idleState = PID_UPPER;
		const auto [cltValid, clt] = Sensor::get(SensorType::Clt);
		if (CONFIG(useIacTableForCoasting) && cltValid) {
			percent_t iacPosForCoasting = interpolate2d("iacCoasting", clt, CONFIG(iacCoastingBins), CONFIG(iacCoasting));
			newValue = interpolateClamped(idlePidLowerRpm, newValue, idlePidLowerRpm + CONFIG(idlePidRpmUpperLimit), iacPosForCoasting, rpm);
		} else {
			// Well, just leave it as is, without PID regulation...
			newValue = engine->engineState.idle.baseIdlePosition;
		}
	}

	return newValue;
}

	int IdleController::getPeriodMs() {
		return GET_PERIOD_LIMITED(&engineConfiguration->idleRpmPid);
	}

	void IdleController::PeriodicTask() {
		efiAssertVoid(OBD_PCM_Processor_Fault, engineConfiguration != NULL, "engineConfiguration pointer");
	/*
	 * Here we have idle logic thread - actual stepper movement is implemented in a separate
	 * working thread,
	 * @see stepper.cpp
	 */

		idlePid.iTermMin = engineConfiguration->idlerpmpid_iTermMin;
		idlePid.iTermMax = engineConfiguration->idlerpmpid_iTermMax;

		SensorResult tps = Sensor::get(SensorType::DriverThrottleIntent);

		engine->engineState.isAutomaticIdle = tps.Valid && engineConfiguration->idleMode == IM_AUTO;

		if (engineConfiguration->isVerboseIAC && engine->engineState.isAutomaticIdle) {
			// todo: print each bit using 'getIdle_state_e' method
			scheduleMsg(logger, "state %d", engine->engineState.idle.idleState);
			idlePid.showPidStatus(logger, "idle");
		}

		if (shouldResetPid) {
			idlePid.reset();
//			alternatorPidResetCounter++;
			shouldResetPid = false;
		}


#if EFI_GPIO_HARDWARE
		// this value is not used yet
		if (CONFIG(clutchDownPin) != GPIO_UNASSIGNED) {
			engine->clutchDownState = efiReadPin(CONFIG(clutchDownPin));
		}
		if (hasAcToggle(PASS_ENGINE_PARAMETER_SIGNATURE)) {
			bool result = getAcToggle(PASS_ENGINE_PARAMETER_SIGNATURE);
			if (engine->acSwitchState != result) {
				engine->acSwitchState = result;
				engine->acSwitchLastChangeTime = getTimeNowUs();
			}
			engine->acSwitchState = result;
		}
		if (CONFIG(clutchUpPin) != GPIO_UNASSIGNED) {
			engine->clutchUpState = efiReadPin(CONFIG(clutchUpPin));
		}
		if (CONFIG(throttlePedalUpPin) != GPIO_UNASSIGNED) {
			engine->engineState.idle.throttlePedalUpState = efiReadPin(CONFIG(throttlePedalUpPin));
		}

		if (engineConfiguration->brakePedalPin != GPIO_UNASSIGNED) {
			engine->brakePedalState = efiReadPin(engineConfiguration->brakePedalPin);
		}
#endif /* EFI_GPIO_HARDWARE */

		finishIdleTestIfNeeded();
		undoIdleBlipIfNeeded();

		const auto [cltValid, clt] = Sensor::get(SensorType::Clt);
#if EFI_SHAFT_POSITION_INPUT
		bool isRunning = engine->rpmCalculator.isRunning(PASS_ENGINE_PARAMETER_SIGNATURE);
#else
		bool isRunning = false;
#endif /* EFI_SHAFT_POSITION_INPUT */
		// cltCorrection is used only for cranking or running in manual mode
		float cltCorrection;
		if (!cltValid)
			cltCorrection = 1.0f;
		// Use separate CLT correction table for cranking
		else if (engineConfiguration->overrideCrankingIacSetting && !isRunning) {
			cltCorrection = interpolate2d("cltCrankingT", clt, config->cltCrankingCorrBins, config->cltCrankingCorr);
		} else {
			// this value would be ignored if running in AUTO mode
			// but we need it while cranking in AUTO mode
			cltCorrection = interpolate2d("cltT", clt, config->cltIdleCorrBins, config->cltIdleCorr);
		}

		percent_t iacPosition;

		if (timeToStopBlip != 0) {
			iacPosition = blipIdlePosition;
			engine->engineState.idle.baseIdlePosition = iacPosition;
			engine->engineState.idle.idleState = BLIP;
		} else if (!isRunning) {
			// during cranking it's always manual mode, PID would make no sense during cranking
			iacPosition = clampPercentValue(cltCorrection * engineConfiguration->crankingIACposition);
			// save cranking position & cycles counter for taper transition
			lastCrankingIacPosition = iacPosition;
			lastCrankingCyclesCounter = engine->rpmCalculator.getRevolutionCounterSinceStart();
			engine->engineState.idle.baseIdlePosition = iacPosition;
		} else {
			if (!tps.Valid || engineConfiguration->idleMode == IM_MANUAL) {
				// let's re-apply CLT correction
				iacPosition = manualIdleController(cltCorrection PASS_ENGINE_PARAMETER_SUFFIX);
			} else {
				iacPosition = automaticIdleController(tps.Value PASS_ENGINE_PARAMETER_SUFFIX);
			}
			
			iacPosition = clampPercentValue(iacPosition);

			// store 'base' iacPosition without adjustments
			engine->engineState.idle.baseIdlePosition = iacPosition;

			float additionalAir = (float)engineConfiguration->iacByTpsTaper;

			if (tps.Valid) {
				iacPosition += interpolateClamped(0.0f, 0.0f, CONFIG(idlePidDeactivationTpsThreshold), additionalAir, tps.Value);
			}

			// taper transition from cranking to running (uint32_t to float conversion is safe here)
			if (engineConfiguration->afterCrankingIACtaperDuration > 0)
				iacPosition = interpolateClamped(lastCrankingCyclesCounter, lastCrankingIacPosition, 
					lastCrankingCyclesCounter + engineConfiguration->afterCrankingIACtaperDuration, iacPosition, 
					engine->rpmCalculator.getRevolutionCounterSinceStart());
		}


		if (engineConfiguration->debugMode == DBG_IDLE_CONTROL) {
			if (engineConfiguration->idleMode == IM_AUTO) {
#if EFI_TUNER_STUDIO
				// see also tsOutputChannels->idlePosition
				idlePid.postState(&tsOutputChannels, 1000000);
				tsOutputChannels.debugIntField4 = engine->engineState.idle.idleState;
#endif /* EFI_TUNER_STUDIO */
			} else {
#if EFI_TUNER_STUDIO
				tsOutputChannels.debugFloatField1 = iacPosition;
				tsOutputChannels.debugIntField1 = iacMotor.getTargetPosition();
#endif /* EFI_TUNER_STUDIO */
			}
		}

		// The threshold is dependent on IAC type (see initIdleHardware())
		if (absF(iacPosition - engine->engineState.idle.currentIdlePosition) < idlePositionSensitivityThreshold) {
			engine->engineState.idle.idleState = (idle_state_e)(engine->engineState.idle.idleState | PWM_PRETTY_CLOSE);
			return; // value is pretty close, let's leave the poor valve alone
		}

		engine->engineState.idle.currentIdlePosition = iacPosition;
		engine->engineState.idle.idleState = (idle_state_e)(engine->engineState.idle.idleState | ADJUSTING);
#if ! EFI_UNIT_TEST
		applyIACposition(engine->engineState.idle.currentIdlePosition);
#endif /* EFI_UNIT_TEST */
	}


IdleController idleControllerInstance;

static void applyPidSettings(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	idlePid.updateFactors(engineConfiguration->idleRpmPid.pFactor, engineConfiguration->idleRpmPid.iFactor, engineConfiguration->idleRpmPid.dFactor);
	iacPidMultMap.init(CONFIG(iacPidMultTable), CONFIG(iacPidMultLoadBins), CONFIG(iacPidMultRpmBins));
}

void setDefaultIdleParameters(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->idleRpmPid.pFactor = 0.1f;
	engineConfiguration->idleRpmPid.iFactor = 0.05f;
	engineConfiguration->idleRpmPid.dFactor = 0.0f;
	engineConfiguration->idleRpmPid.periodMs = 10;

	engineConfiguration->idlerpmpid_iTermMin = -200;
	engineConfiguration->idlerpmpid_iTermMax =  200;
}

#if ! EFI_UNIT_TEST

void onConfigurationChangeIdleCallback(engine_configuration_s *previousConfiguration) {
	shouldResetPid = !idlePid.isSame(&previousConfiguration->idleRpmPid);
	idleSolenoidOpen.setFrequency(CONFIG(idle).solenoidFrequency);
	idleSolenoidClose.setFrequency(CONFIG(idle).solenoidFrequency);
}

void setTargetIdleRpm(int value) {
	setTargetRpmCurve(value PASS_ENGINE_PARAMETER_SUFFIX);
	scheduleMsg(logger, "target idle RPM %d", value);
	showIdleInfo();
}

void setIdleOffset(float value)  {
	engineConfiguration->idleRpmPid.offset = value;
	showIdleInfo();
}

void setIdlePFactor(float value) {
	engineConfiguration->idleRpmPid.pFactor = value;
	applyPidSettings();
	showIdleInfo();
}

void setIdleIFactor(float value) {
	engineConfiguration->idleRpmPid.iFactor = value;
	applyPidSettings();
	showIdleInfo();
}

void setIdleDFactor(float value) {
	engineConfiguration->idleRpmPid.dFactor = value;
	applyPidSettings();
	showIdleInfo();
}

void setIdleDT(int value) {
	engineConfiguration->idleRpmPid.periodMs = value;
	applyPidSettings();
	showIdleInfo();
}

/**
 * Idle test would activate the solenoid for three seconds
 */
void startIdleBench(void) {
	timeToStopIdleTest = getTimeNowUs() + MS2US(3000); // 3 seconds
	scheduleMsg(logger, "idle valve bench test");
	showIdleInfo();
}

static void applyIdleSolenoidPinState(int stateIndex, PwmConfig *state) /* pwm_gen_callback */ {
	efiAssertVoid(CUSTOM_ERR_6645, stateIndex < PWM_PHASE_MAX_COUNT, "invalid stateIndex");
	efiAssertVoid(CUSTOM_ERR_6646, state->multiChannelStateSequence.waveCount == 1, "invalid idle waveCount");
	OutputPin *output = state->outputPins[0];
	int value = state->multiChannelStateSequence.getChannelState(/*channelIndex*/0, stateIndex);
	if (!value /* always allow turning solenoid off */ ||
			(GET_RPM_VALUE != 0 || timeToStopIdleTest != 0) /* do not run solenoid unless engine is spinning or bench testing in progress */
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

void stopIdleHardware(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if EFI_PROD_CODE
	brain_pin_markUnused(activeConfiguration.stepperEnablePin);
	brain_pin_markUnused(activeConfiguration.idle.stepperStepPin);
	brain_pin_markUnused(activeConfiguration.idle.solenoidPin);
	brain_pin_markUnused(activeConfiguration.secondSolenoidPin);
//	brain_pin_markUnused(activeConfiguration.idle.);
//	brain_pin_markUnused(activeConfiguration.idle.);
//	brain_pin_markUnused(activeConfiguration.idle.);
//	brain_pin_markUnused(activeConfiguration.idle.);
#endif /* EFI_PROD_CODE */
}

void initIdleHardware(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (CONFIG(useStepperIdle)) {
		StepperHw* hw;

		if (CONFIG(useHbridges)) {
			auto motorA = initDcMotor(0 PASS_ENGINE_PARAMETER_SUFFIX);
			auto motorB = initDcMotor(1 PASS_ENGINE_PARAMETER_SUFFIX);

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
	} else if (!engineConfiguration->useETBforIdleControl) {
		/**
		 * Start PWM for idleValvePin
		 */
		// todo: even for double-solenoid mode we can probably use same single SimplePWM
		// todo: open question why do we pass 'OutputPin' into 'startSimplePwmExt' if we have custom applyIdleSolenoidPinState listener anyway?
		if (!CONFIG(isDoubleSolenoidIdle)) {
			startSimplePwmExt(&idleSolenoidOpen, "Idle Valve",
					&engine->executor,
					CONFIG(idle).solenoidPin, &enginePins.idleSolenoidPin,
					CONFIG(idle).solenoidFrequency, PERCENT_TO_DUTY(CONFIG(manIdlePosition)),
					(pwm_gen_callback*)applyIdleSolenoidPinState);
		} else {
			startSimplePwmExt(&idleSolenoidOpen, "Idle Valve Open",
					&engine->executor,
					CONFIG(idle).solenoidPin, &enginePins.idleSolenoidPin,
					CONFIG(idle).solenoidFrequency, PERCENT_TO_DUTY(CONFIG(manIdlePosition)),
					(pwm_gen_callback*)applyIdleSolenoidPinState);

			startSimplePwmExt(&idleSolenoidClose, "Idle Valve Close",
					&engine->executor,
					CONFIG(secondSolenoidPin), &enginePins.secondIdleSolenoidPin,
					CONFIG(idle).solenoidFrequency, PERCENT_TO_DUTY(CONFIG(manIdlePosition)),
					(pwm_gen_callback*)applyIdleSolenoidPinState);
		}
		idlePositionSensitivityThreshold = 0.0f;
	}
}

#endif /* EFI_UNIT_TEST */

void startIdleThread(Logging*sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX) {
	logger = sharedLogger;
	INJECT_ENGINE_REFERENCE(&idleControllerInstance);

	idlePid.initPidClass(&engineConfiguration->idleRpmPid);

#if ! EFI_UNIT_TEST
	// todo: we still have to explicitly init all hardware on start in addition to handling configuration change via
	// 'applyNewHardwareSettings' todo: maybe unify these two use-cases?
	initIdleHardware(PASS_ENGINE_PARAMETER_SIGNATURE);
#endif /* EFI_UNIT_TEST */

	DISPLAY_STATE(Engine)
	DISPLAY_TEXT(Idle_State);
	engine->engineState.idle.DISPLAY_FIELD(idleState) = INIT;
	DISPLAY_TEXT(EOL);
	DISPLAY_TEXT(Base_Position);
	engine->engineState.idle.DISPLAY_FIELD(baseIdlePosition) = -100.0f;
	DISPLAY_TEXT(Position_with_Adjustments);
	engine->engineState.idle.DISPLAY_FIELD(currentIdlePosition) = -100.0f;
	DISPLAY_TEXT(EOL);
	DISPLAY_TEXT(EOL);
	DISPLAY_SENSOR(TPS);
	DISPLAY_TEXT(EOL);
	DISPLAY_TEXT(Throttle_Up_State);
	DISPLAY(DISPLAY_FIELD(throttlePedalUpState));
	DISPLAY(DISPLAY_CONFIG(throttlePedalUpPin));

	DISPLAY_TEXT(eol);
	DISPLAY(DISPLAY_IF(isAutomaticIdle))

		DISPLAY_STATE(idle_pid)
		DISPLAY_TEXT(Output);
		DISPLAY(DISPLAY_FIELD(output));
		DISPLAY_TEXT(iTerm);
		DISPLAY(DISPLAY_FIELD(iTerm));
		DISPLAY_TEXT(eol);

		DISPLAY_TEXT(Settings);
		DISPLAY(DISPLAY_CONFIG(IDLERPMPID_PFACTOR));
		DISPLAY(DISPLAY_CONFIG(IDLERPMPID_IFACTOR));
		DISPLAY(DISPLAY_CONFIG(IDLERPMPID_DFACTOR));
		DISPLAY(DISPLAY_CONFIG(IDLERPMPID_OFFSET));


		DISPLAY_TEXT(eol);
		DISPLAY_TEXT(ETB_Idle);
		DISPLAY_STATE(Engine)
		DISPLAY(DISPLAY_FIELD(etbIdleAddition));
	/* DISPLAY_ELSE */
			DISPLAY_TEXT(Manual_idle_control);
	/* DISPLAY_ENDIF */


	//scheduleMsg(logger, "initial idle %d", idlePositionController.value);

	idleControllerInstance.Start();

#if ! EFI_UNIT_TEST
	// this is neutral/no gear switch input. on Miata it's wired both to clutch pedal and neutral in gearbox
	// this switch is not used yet
	if (CONFIG(clutchDownPin) != GPIO_UNASSIGNED) {
		efiSetPadMode("clutch down switch", CONFIG(clutchDownPin),
				getInputMode(CONFIG(clutchDownPinMode)));
	}

	if (CONFIG(clutchUpPin) != GPIO_UNASSIGNED) {
		efiSetPadMode("clutch up switch", CONFIG(clutchUpPin),
				getInputMode(CONFIG(clutchUpPinMode)));
	}

	if (CONFIG(throttlePedalUpPin) != GPIO_UNASSIGNED) {
		efiSetPadMode("throttle pedal up switch", CONFIG(throttlePedalUpPin),
				getInputMode(CONFIG(throttlePedalUpPinMode)));
	}

	if (engineConfiguration->brakePedalPin != GPIO_UNASSIGNED) {
#if EFI_PROD_CODE
		efiSetPadMode("brake pedal switch", engineConfiguration->brakePedalPin,
				getInputMode(engineConfiguration->brakePedalPinMode));
#endif /* EFI_PROD_CODE */
	}

	addConsoleAction("idleinfo", showIdleInfo);

	addConsoleActionII("blipidle", blipIdle);

	// split this whole file into manual controller and auto controller? move these commands into the file
	// which would be dedicated to just auto-controller?

	addConsoleAction("idlebench", startIdleBench);
#endif /* EFI_UNIT_TEST */
	applyPidSettings(PASS_ENGINE_PARAMETER_SIGNATURE);
}

#endif /* EFI_IDLE_CONTROL */
