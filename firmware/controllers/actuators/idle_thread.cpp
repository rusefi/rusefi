/**
 * @file    idle_thread.cpp
 * @brief   Idle Air Control valve thread.
 *
 * This thread looks at current RPM and decides if it should increase or decrease IAC duty cycle.
 * This file is has the hardware & scheduling logic, desired idle level lives separately
 *
 *
 * @date May 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
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

#if EFI_IDLE_CONTROL || defined(__DOXYGEN__)
#include "engine_configuration.h"
#include "rpm_calculator.h"
#include "pwm_generator.h"
#include "idle_thread.h"
#include "pin_repository.h"
#include "engine.h"
#include "periodic_controller.h"
#include "stepper.h"
#include "allsensors.h"

static Logging *logger;
#if EFI_TUNER_STUDIO || defined(__DOXYGEN__)
extern TunerStudioOutputChannels tsOutputChannels;
#endif /* EFI_TUNER_STUDIO */
EXTERN_ENGINE
;

static bool shouldResetPid = false;
// we might reset PID state when the state is changed, but only if needed (See autoIdle())
static bool mightResetPid = false;

#if EFI_IDLE_INCREMENTAL_PID_CIC || defined(__DOXYGEN__)
// Use new PID with CIC integrator
static PidCic idlePid(&engineConfiguration->idleRpmPid);
#else
static Pid idlePid(&engineConfiguration->idleRpmPid);
#endif /* EFI_IDLE_INCREMENTAL_PID_CIC */

// todo: extract interface for idle valve hardware, with solenoid and stepper implementations?
static SimplePwm idleSolenoid("idle");

static StepperMotor iacMotor;

static uint32_t lastCrankingCyclesCounter = 0;
static float lastCrankingIacPosition;

typedef enum {
	INIT = 0,
	TPS_THRESHOLD = 1,
	RPM_DEAD_ZONE = 2,
	PID_VALUE = 3,
	PWM_PRETTY_CLOSE = 4,
	PID_UPPER = 5,
	ADJUSTING = 8,
	BLIP = 16,

} idle_state_e;

static idle_state_e idleState = INIT;

/**
 * that's current position with CLT and IAT corrections
 */
static percent_t currentIdlePosition = -100.0f;
/**
 * the same as currentIdlePosition, but without adjustments (iacByTpsTaper, afterCrankingIACtaperDuration)
 */
static percent_t baseIdlePosition = currentIdlePosition;
/**
 * When the IAC position value change is insignificant (lower than this threshold), leave the poor valve alone
 * todo: why do we have this logic? is this ever useful?
 * See
 */
static percent_t idlePositionSensitivityThreshold = 0.0f;

void idleDebug(const char *msg, percent_t value) {
	scheduleMsg(logger, "idle debug: %s%.2f", msg, value);
}

static void showIdleInfo(void) {
	const char * idleModeStr = getIdle_mode_e(engineConfiguration->idleMode);
	scheduleMsg(logger, "idleMode=%s position=%.2f isStepper=%s", idleModeStr,
			getIdlePosition(), boolToString(CONFIGB(useStepperIdle)));

	if (CONFIGB(useStepperIdle)) {
		scheduleMsg(logger, "directionPin=%s reactionTime=%.2f", hwPortname(CONFIGB(idle).stepperDirectionPin),
				engineConfiguration->idleStepperReactionTime);
		scheduleMsg(logger, "stepPin=%s steps=%d", hwPortname(CONFIGB(idle).stepperStepPin),
				engineConfiguration->idleStepperTotalSteps);
		scheduleMsg(logger, "enablePin=%s/%d", hwPortname(engineConfiguration->stepperEnablePin),
				engineConfiguration->stepperEnablePinMode);
	} else {
		scheduleMsg(logger, "idle valve freq=%d on %s", CONFIGB(idle).solenoidFrequency,
				hwPortname(CONFIGB(idle).solenoidPin));
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
	if (CONFIGB(useStepperIdle)) {
		iacMotor.setTargetPosition(position / 100 * engineConfiguration->idleStepperTotalSteps);
	} else {
		/**
		 * currently idle level is an percent value (0-100 range), and PWM takes a float in the 0..1 range
		 * todo: unify?
		 */
		idleSolenoid.setSimplePwmDutyCycle(position / 100.0);
	}
}

static percent_t manualIdleController(float cltCorrection) {

	percent_t correctedPosition = cltCorrection * CONFIGB(manIdlePosition);

	// let's put the value into the right range
	correctedPosition = maxF(correctedPosition, 0.01);
	correctedPosition = minF(correctedPosition, 99.9);

	return correctedPosition;
}

void setIdleValvePosition(int positionPercent) {
	if (positionPercent < 1 || positionPercent > 99)
		return;
	scheduleMsg(logger, "setting idle valve position %d", positionPercent);
	showIdleInfo();
	// todo: this is not great that we have to write into configuration here
	CONFIGB(manIdlePosition) = positionPercent;
}

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

percent_t getIdlePosition(void) {
	return currentIdlePosition;
}

/**
 * @return idle valve position percentage for automatic closed loop mode
 */
static percent_t automaticIdleController() {
	percent_t tpsPos = getTPS(PASS_ENGINE_PARAMETER_SIGNATURE);
	if (tpsPos > CONFIGB(idlePidDeactivationTpsThreshold)) {
		// Don't store old I and D terms if PID doesn't work anymore.
		// Otherwise they will affect the idle position much later, when the throttle is closed.
		if (mightResetPid) {
			mightResetPid = false;
			shouldResetPid = true;
		}

		idleState = TPS_THRESHOLD;
		// just leave IAC position as is (but don't return currentIdlePosition - it may already contain additionalAir)
		return baseIdlePosition;
	}

	// get Target RPM for Auto-PID from a separate table
	int targetRpm = getTargetRpmForIdleCorrection(PASS_ENGINE_PARAMETER_SIGNATURE);

	// check if within the dead zone
	int rpm = GET_RPM();
	if (absI(rpm - targetRpm) <= CONFIG(idlePidRpmDeadZone)) {
		idleState = RPM_DEAD_ZONE;
		// current RPM is close enough, no need to change anything
		return baseIdlePosition;
	}

	// When rpm < targetRpm, there's a risk of dropping RPM too low - and the engine dies out.
	// So PID reaction should be increased by adding extra percent to PID-error:
	percent_t errorAmpCoef = 1.0f;
	if (rpm < targetRpm)
		errorAmpCoef += (float)CONFIG(pidExtraForLowRpm) / PERCENT_MULT;
	// If errorAmpCoef > 1.0, then PID thinks that RPM is lower than it is, and controls IAC more aggressively
	idlePid.setErrorAmplification(errorAmpCoef);

	percent_t newValue = idlePid.getOutput(targetRpm, rpm, engineConfiguration->idleRpmPid.periodMs);
	idleState = PID_VALUE;

	// the state of PID has been changed, so we might reset it now, but only when needed (see idlePidDeactivationTpsThreshold)
	mightResetPid = true;

#if EFI_IDLE_INCREMENTAL_PID_CIC || defined(__DOXYGEN__)
	// Treat the 'newValue' as if it contains not an actual IAC position, but an incremental delta.
	// So we add this delta to the base IAC position, with a smooth taper for TPS transients.
	newValue = baseIdlePosition + interpolateClamped(0.0f, newValue, CONFIGB(idlePidDeactivationTpsThreshold), 0.0f, tpsPos);

	// apply the PID limits
	newValue = maxF(newValue, CONFIG(idleRpmPid.minValue));
	newValue = minF(newValue, CONFIG(idleRpmPid.maxValue));
#endif /* EFI_IDLE_INCREMENTAL_PID_CIC */

	// Interpolate to the manual position when RPM is close to the upper RPM limit (if idlePidRpmUpperLimit is set).
	// If RPM increases and the throttle is closed, then we're in coasting mode, and we should smoothly disable auto-pid.
	// If we just leave IAC at baseIdlePosition (as in case of TPS deactivation threshold), RPM would get stuck. 
	// That's why there's 'useIacTableForCoasting' setting which involves a separate IAC position table for coasting (iacCoasting).
	// Currently it's user-defined. But eventually we'll use a real calculated and stored IAC position instead.
	int idlePidLowerRpm = targetRpm + CONFIG(idlePidRpmDeadZone);
	if (CONFIG(idlePidRpmUpperLimit) > 0) {
		idleState = PID_UPPER;
		if (CONFIGB(useIacTableForCoasting) && !cisnan(engine->sensors.clt)) {
			percent_t iacPosForCoasting = interpolate2d("iacCoasting", engine->sensors.clt, CONFIG(iacCoastingBins), CONFIG(iacCoasting), CLT_CURVE_SIZE);
			newValue = interpolateClamped(idlePidLowerRpm, newValue, idlePidLowerRpm + CONFIG(idlePidRpmUpperLimit), iacPosForCoasting, rpm);
		} else {
			// Well, just leave it as is, without PID regulation...
			newValue = baseIdlePosition;
		}
	}

	return newValue;
}

class IdleController : public PeriodicController<UTILITY_THREAD_STACK_SIZE> {
public:
	IdleController() : PeriodicController("IdleValve") { }
private:
	void PeriodicTask(efitime_t nowNt) override	{
		UNUSED(nowNt);
		setPeriod(NOT_TOO_OFTEN(10 /* ms */, engineConfiguration->idleRpmPid.periodMs));

	/*
	 * Here we have idle logic thread - actual stepper movement is implemented in a separate
	 * working thread,
	 * @see stepper.cpp
	 */

		if (engineConfiguration->isVerboseIAC && engineConfiguration->idleMode == IM_AUTO) {
			scheduleMsg(logger, "state %d", idleState);
			idlePid.showPidStatus(logger, "idle");
		}

		if (shouldResetPid) {
			idlePid.reset();
//			alternatorPidResetCounter++;
			shouldResetPid = false;
		}


#if EFI_PROD_CODE || defined(__DOXYGEN__)
		// this value is not used yet
		if (CONFIGB(clutchDownPin) != GPIO_UNASSIGNED) {
			engine->clutchDownState = efiReadPin(CONFIGB(clutchDownPin));
		}
		if (hasAcToggle(PASS_ENGINE_PARAMETER_SIGNATURE)) {
			engine->acSwitchState = getAcToggle(PASS_ENGINE_PARAMETER_SIGNATURE);
		}
		if (CONFIGB(clutchUpPin) != GPIO_UNASSIGNED) {
			engine->clutchUpState = efiReadPin(CONFIGB(clutchUpPin));
		}

		if (engineConfiguration->brakePedalPin != GPIO_UNASSIGNED) {
			engine->brakePedalState = efiReadPin(engineConfiguration->brakePedalPin);
		}
#endif /* EFI_PROD_CODE */

		finishIdleTestIfNeeded();
		undoIdleBlipIfNeeded();

		float clt = engine->sensors.clt;
#if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
		bool isRunning = engine->rpmCalculator.isRunning(PASS_ENGINE_PARAMETER_SIGNATURE);
#else
		bool isRunning = false;
#endif /* EFI_SHAFT_POSITION_INPUT */
		// cltCorrection is used only for cranking or running in manual mode
		float cltCorrection;
		if (cisnan(clt))
			cltCorrection = 1.0f;
		// Use separate CLT correction table for cranking
		else if (engineConfiguration->overrideCrankingIacSetting && !isRunning) {
			cltCorrection = interpolate2d("cltCrankingT", clt, config->cltCrankingCorrBins, config->cltCrankingCorr, CLT_CRANKING_CURVE_SIZE) / PERCENT_MULT;
		} else {
			// this value would be ignored if running in AUTO mode
			// but we need it while cranking in AUTO mode
			cltCorrection = interpolate2d("cltT", clt, config->cltIdleCorrBins, config->cltIdleCorr, CLT_CURVE_SIZE) / PERCENT_MULT;
		}

		percent_t iacPosition;

		if (timeToStopBlip != 0) {
			iacPosition = blipIdlePosition;
			baseIdlePosition = iacPosition;
			idleState = BLIP;
		} else if (!isRunning) {
			// during cranking it's always manual mode, PID would make no sence during cranking
			iacPosition = cltCorrection * engineConfiguration->crankingIACposition;
			// save cranking position & cycles counter for taper transition
			lastCrankingIacPosition = iacPosition;
			lastCrankingCyclesCounter = engine->rpmCalculator.getRevolutionCounterSinceStart();
			baseIdlePosition = iacPosition;
		} else {
			if (engineConfiguration->idleMode == IM_MANUAL) {
				// let's re-apply CLT correction
				iacPosition = manualIdleController(cltCorrection);
			} else {
				iacPosition = automaticIdleController();
			}
			
			// store 'base' iacPosition without adjustments
			baseIdlePosition = iacPosition;
			
			percent_t tpsPos = getTPS(PASS_ENGINE_PARAMETER_SIGNATURE);
			float additionalAir = (float)engineConfiguration->iacByTpsTaper;
			iacPosition += interpolateClamped(0.0f, 0.0f, CONFIGB(idlePidDeactivationTpsThreshold), additionalAir, tpsPos);

			// taper transition from cranking to running (uint32_t to float conversion is safe here)
			if (engineConfiguration->afterCrankingIACtaperDuration > 0)
				iacPosition = interpolateClamped(lastCrankingCyclesCounter, lastCrankingIacPosition, 
					lastCrankingCyclesCounter + engineConfiguration->afterCrankingIACtaperDuration, iacPosition, 
					engine->rpmCalculator.getRevolutionCounterSinceStart());
		}


		if (engineConfiguration->debugMode == DBG_IDLE_CONTROL) {
			if (engineConfiguration->idleMode == IM_AUTO) {
#if EFI_TUNER_STUDIO || defined(__DOXYGEN__)
				// see also tsOutputChannels->idlePosition
				idlePid.postState(&tsOutputChannels, 1000000);
				tsOutputChannels.debugIntField4 = idleState;
#endif /* EFI_TUNER_STUDIO */
			} else {
#if EFI_TUNER_STUDIO || defined(__DOXYGEN__)
				tsOutputChannels.debugFloatField1 = iacPosition;
				tsOutputChannels.debugIntField1 = iacMotor.getTargetPosition();
#endif /* EFI_TUNER_STUDIO */
			}
		}

		// The threshold is dependent on IAC type (see initIdleHardware())
		if (absF(iacPosition - currentIdlePosition) < idlePositionSensitivityThreshold) {
			idleState = (idle_state_e)(idleState | PWM_PRETTY_CLOSE);
			return; // value is pretty close, let's leave the poor valve alone
		}

		currentIdlePosition = iacPosition;
		idleState = (idle_state_e)(idleState | ADJUSTING);
		applyIACposition(currentIdlePosition);
	}
};

static IdleController instance;

void setTargetIdleRpm(int value) {
	setTargetRpmCurve(value PASS_ENGINE_PARAMETER_SUFFIX);
	scheduleMsg(logger, "target idle RPM %d", value);
	showIdleInfo();
}

static void apply(void) {
	idlePid.updateFactors(engineConfiguration->idleRpmPid.pFactor, engineConfiguration->idleRpmPid.iFactor, engineConfiguration->idleRpmPid.dFactor);
}

void setIdleOffset(float value)  {
	engineConfiguration->idleRpmPid.offset = value;
	showIdleInfo();
}

void setIdlePFactor(float value) {
	engineConfiguration->idleRpmPid.pFactor = value;
	apply();
	showIdleInfo();
}

void setIdleIFactor(float value) {
	engineConfiguration->idleRpmPid.iFactor = value;
	apply();
	showIdleInfo();
}

void setIdleDFactor(float value) {
	engineConfiguration->idleRpmPid.dFactor = value;
	apply();
	showIdleInfo();
}

void setIdleDT(int value) {
	engineConfiguration->idleRpmPid.periodMs = value;
	apply();
	showIdleInfo();
}

void onConfigurationChangeIdleCallback(engine_configuration_s *previousConfiguration) {
	shouldResetPid = !idlePid.isSame(&previousConfiguration->idleRpmPid);
	idleSolenoid.setFrequency(CONFIGB(idle).solenoidFrequency);
}

/**
 * Idle test would activate the solenoid for three seconds
 */
void startIdleBench(void) {
	timeToStopIdleTest = getTimeNowUs() + MS2US(3000); // 3 seconds
	scheduleMsg(logger, "idle valve bench test");
	showIdleInfo();
}

void setDefaultIdleParameters(void) {
	engineConfiguration->idleRpmPid.pFactor = 0.1f;
	engineConfiguration->idleRpmPid.iFactor = 0.05f;
	engineConfiguration->idleRpmPid.dFactor = 0.0f;
	engineConfiguration->idleRpmPid.periodMs = 10;
}

static void applyIdleSolenoidPinState(PwmConfig *unused, int stateIndex, PwmConfig *state) /* pwm_gen_callback */ {
	efiAssertVoid(CUSTOM_ERR_6645, stateIndex < PWM_PHASE_MAX_COUNT, "invalid stateIndex");
	efiAssertVoid(CUSTOM_ERR_6646, state->multiWave.waveCount == 1, "invalid idle waveCount");
	OutputPin *output = state->outputPins[0];
	int value = state->multiWave.getChannelState(/*channelIndex*/0, stateIndex);
	if (!value /* always allow turning solenoid off */ ||
			(GET_RPM_VALUE != 0 || timeToStopIdleTest != 0) /* do not run solenoid unless engine is spinning or bench testing in progress */
			) {
		output->setValue(value);
	}
}

static void initIdleHardware() {
	if (CONFIGB(useStepperIdle)) {
		iacMotor.initialize(CONFIGB(idle).stepperStepPin,
				CONFIGB(idle).stepperDirectionPin,
				engineConfiguration->stepperDirectionPinMode,
				engineConfiguration->idleStepperReactionTime,
				engineConfiguration->idleStepperTotalSteps,
				engineConfiguration->stepperEnablePin, engineConfiguration->stepperEnablePinMode,
				logger);
		// This greatly improves PID accuracy for steppers with a small number of steps
		idlePositionSensitivityThreshold = 1.0f / engineConfiguration->idleStepperTotalSteps;
	} else {
		/**
		 * Start PWM for idleValvePin
		 */
		startSimplePwmExt(&idleSolenoid, "Idle Valve",
				&engine->executor,
				CONFIGB(idle).solenoidPin, &enginePins.idleSolenoidPin,
				CONFIGB(idle).solenoidFrequency, CONFIGB(manIdlePosition) / 100,
				(pwm_gen_callback*)applyIdleSolenoidPinState);
		idlePositionSensitivityThreshold = 0.0f;
	}
}

void startIdleThread(Logging*sharedLogger) {
	logger = sharedLogger;

	// todo: re-initialize idle pins on the fly
	initIdleHardware();

	//scheduleMsg(logger, "initial idle %d", idlePositionController.value);

	instance.Start();

	// this is neutral/no gear switch input. on Miata it's wired both to clutch pedal and neutral in gearbox
	// this switch is not used yet
	if (CONFIGB(clutchDownPin) != GPIO_UNASSIGNED) {
		efiSetPadMode("clutch down switch", CONFIGB(clutchDownPin),
				getInputMode(CONFIGB(clutchDownPinMode)));
	}

	if (CONFIGB(clutchUpPin) != GPIO_UNASSIGNED) {
		efiSetPadMode("clutch up switch", CONFIGB(clutchUpPin),
				getInputMode(CONFIGB(clutchUpPinMode)));
	}

	if (engineConfiguration->brakePedalPin != GPIO_UNASSIGNED) {
#if EFI_PROD_CODE || defined(__DOXYGEN__)
		efiSetPadMode("brake pedal switch", engineConfiguration->brakePedalPin,
				getInputMode(engineConfiguration->brakePedalPinMode));
#endif /* EFI_PROD_CODE */
	}

	addConsoleAction("idleinfo", showIdleInfo);

	addConsoleActionII("blipidle", blipIdle);

	// split this whole file into manual controller and auto controller? move these commands into the file
	// which would be dedicated to just auto-controller?

	addConsoleAction("idlebench", startIdleBench);
	apply();
}

#endif /* EFI_IDLE_CONTROL */
