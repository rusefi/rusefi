/**
 * @file    idle_thread.cpp
 * @brief   Idle Air Control valve thread.
 *
 * This thread looks at current RPM and decides if it should increase or decrease IAC duty cycle.
 * This file is has the hardware & scheduling logic, desired idle level lives separately
 *
 *
 * @date May 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
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

#include "main.h"
#include "rpm_calculator.h"
#include "pwm_generator.h"
#include "idle_thread.h"
#include "pin_repository.h"
#include "engine_configuration.h"
#include "engine.h"
#include "stepper.h"

#if EFI_IDLE_CONTROL || defined(__DOXYGEN__)
#include "allsensors.h"

static THD_WORKING_AREA(ivThreadStack, UTILITY_THREAD_STACK_SIZE);

static Logging *logger;
extern TunerStudioOutputChannels tsOutputChannels;
EXTERN_ENGINE
;

static bool shouldResetPid = false;

static Pid idlePid(&engineConfiguration->idleRpmPid, 1, 99);

// todo: extract interface for idle valve hardware, with solenoid and stepper implementations?
static SimplePwm idleSolenoid;

static StepperMotor iacMotor;

static int adjustedTargetRpm;

/**
 * that's current position with CLT and IAT corrections
 */
static percent_t currentIdlePosition = -100.0f;

void idleDebug(const char *msg, percent_t value) {
	scheduleMsg(logger, "idle debug: %s%f", msg, value);
}


static void showIdleInfo(void) {
	const char * idleModeStr = getIdle_mode_e(engineConfiguration->idleMode);
	scheduleMsg(logger, "idleMode=%s position=%f isStepper=%s", idleModeStr,
			getIdlePosition(), boolToString(boardConfiguration->useStepperIdle));

	if (boardConfiguration->useStepperIdle) {
		scheduleMsg(logger, "directionPin=%s reactionTime=%f", hwPortname(boardConfiguration->idle.stepperDirectionPin),
				engineConfiguration->idleStepperReactionTime);
		scheduleMsg(logger, "stepPin=%s steps=%d", hwPortname(boardConfiguration->idle.stepperStepPin),
				engineConfiguration->idleStepperTotalSteps);
		scheduleMsg(logger, "enablePin=%s/%d", hwPortname(engineConfiguration->stepperEnablePin),
				engineConfiguration->stepperEnablePinMode);
	} else {
		scheduleMsg(logger, "idle valve freq=%d on %s", boardConfiguration->idle.solenoidFrequency,
				hwPortname(boardConfiguration->idle.solenoidPin));
	}


	if (engineConfiguration->idleMode == IM_AUTO) {
		idlePid.showPidStatus(logger, "idle", engineConfiguration->idleDT);
	}
}

void setIdleMode(idle_mode_e value) {
	engineConfiguration->idleMode = value ? IM_AUTO : IM_MANUAL;
	showIdleInfo();
}

static void applyIACposition(percent_t position) {
	if (boardConfiguration->useStepperIdle) {
		iacMotor.setTargetPosition(position / 100 * engineConfiguration->idleStepperTotalSteps);
	} else {
		/**
		 * currently idle level is an percent value (0-100 range), and PWM takes a float in the 0..1 range
		 * todo: unify?
		 */
		idleSolenoid.setSimplePwmDutyCycle(position / 100.0);
	}
}

static float manualIdleController(float cltCorrection) {

	percent_t correctedPosition = cltCorrection * boardConfiguration->manIdlePosition;

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
	boardConfiguration->manIdlePosition = positionPercent;
}

static int blipIdlePosition;
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

static percent_t currentIdleValve = -1;

percent_t getIdlePosition(void) {
	if (engineConfiguration->idleMode == IM_AUTO) {
		return currentIdleValve;
	} else {
		return boardConfiguration->manIdlePosition;
	}
}

static float autoIdle(float cltCorrection) {
	if (getTPS(PASS_ENGINE_PARAMETER_SIGNATURE) > boardConfiguration->idlePidDeactivationTpsThreshold)
		return currentIdlePosition;

	adjustedTargetRpm = engineConfiguration->targetIdleRpm * cltCorrection;

	percent_t newValue = idlePid.getValue(adjustedTargetRpm, getRpmE(engine), engineConfiguration->idleDT);

	return newValue;
}

static msg_t ivThread(int param) {
	(void) param;
	chRegSetThreadName("IdleValve");

	/*
	 * Here we have idle logic thread - actual stepper movement is implemented in a separate
	 * working thread,
	 * @see stepper.cpp
	 */

	while (true) {
		// todo: in auto mode, speel should be taken from idleDTe
		chThdSleepMilliseconds(boardConfiguration->idleThreadPeriod);

		if (shouldResetPid) {
			idlePid.reset();
//			alternatorPidResetCounter++;
			shouldResetPid = false;
		}



		// this value is not used yet
		if (boardConfiguration->clutchDownPin != GPIO_UNASSIGNED) {
			engine->clutchDownState = efiReadPin(boardConfiguration->clutchDownPin);
		}
		if (boardConfiguration->clutchUpPin != GPIO_UNASSIGNED) {
			engine->clutchUpState = efiReadPin(boardConfiguration->clutchUpPin);
		}

		if (engineConfiguration->brakePedalPin != GPIO_UNASSIGNED) {
			engine->brakePedalState = efiReadPin(engineConfiguration->brakePedalPin);
		}

		finishIdleTestIfNeeded();
		undoIdleBlipIfNeeded();

		float cltCorrection = interpolate2d(engine->sensors.clt, config->cltIdleCorrBins, config->cltIdleCorr,
		CLT_CURVE_SIZE) / PERCENT_MULT;

		float iacPosition;

		if (timeToStopBlip != 0) {
			iacPosition = blipIdlePosition;
		} else if (isCrankingE(engine)) {
			// during cranking it's always manual mode, PID would make no sence during cranking
			iacPosition = cltCorrection * engineConfiguration->crankingIACposition;
		} else if (engineConfiguration->idleMode == IM_MANUAL) {
			// let's re-apply CLT correction
			iacPosition = manualIdleController(cltCorrection);
		} else {
			iacPosition = autoIdle(cltCorrection);
		}

		if (absF(iacPosition - currentIdlePosition) < 1) {
			continue; // value is pretty close, let's leave the poor valve alone
		}

		if (engineConfiguration->debugMode == DBG_IDLE_CONTROL) {
			tsOutputChannels.debugFloatField1 = iacPosition;
#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)
			if (engineConfiguration->idleMode == IM_AUTO) {
				idlePid.postState(&tsOutputChannels);
			}
#endif
		}

		if (engineConfiguration->isVerboseIAC && engineConfiguration->idleMode == IM_AUTO) {
			idlePid.showPidStatus(logger, "idle", engineConfiguration->idleDT);

		}

		currentIdlePosition = iacPosition;

		applyIACposition(currentIdlePosition);
	}
#if defined __GNUC__
	return -1;
#endif
}

void setTargetIdleRpm(int value) {
	engineConfiguration->targetIdleRpm = value;
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
	engineConfiguration->idleDT = value;
	apply();
	showIdleInfo();
}

void onConfigurationChangeIdleCallback(engine_configuration_s *previousConfiguration) {
	shouldResetPid = !idlePid.isSame(&previousConfiguration->idleRpmPid);
	idlePid.minResult = engineConfiguration->idleValvePidMin;
	idlePid.maxResult = engineConfiguration->idleValvePidMax;
}

void startIdleBench(void) {
	timeToStopIdleTest = getTimeNowUs() + MS2US(3000); // 3 seconds
	scheduleMsg(logger, "idle valve bench test");
	showIdleInfo();
}

void setDefaultIdleParameters(void) {
	engineConfiguration->idleRpmPid.pFactor = 0.1f;
	engineConfiguration->idleRpmPid.iFactor = 0.05f;
	engineConfiguration->idleRpmPid.dFactor = 0.0f;
	engineConfiguration->idleDT = 10;
}

static void applyIdleSolenoidPinState(PwmConfig *state, int stateIndex) {
	efiAssertVoid(stateIndex < PWM_PHASE_MAX_COUNT, "invalid stateIndex");
	efiAssertVoid(state->multiWave.waveCount == 1, "invalid idle waveCount");
	OutputPin *output = state->outputPins[0];
	int value = state->multiWave.waves[0].pinStates[stateIndex];
	if (!value /* always allow turning solenoid off */ ||
			(engine->rpmCalculator.rpmValue != 0 || timeToStopIdleTest != 0) /* do not run solenoid unless engine is spinning or bench testing in progress */
			) {
		output->setValue(value);
	}
}

static void initIdleHardware() {
	if (boardConfiguration->useStepperIdle) {
		iacMotor.initialize(boardConfiguration->idle.stepperStepPin, boardConfiguration->idle.stepperDirectionPin,
				engineConfiguration->idleStepperReactionTime, engineConfiguration->idleStepperTotalSteps,
				engineConfiguration->stepperEnablePin);
	} else {
		/**
		 * Start PWM for idleValvePin
		 */
		startSimplePwmExt(&idleSolenoid, "Idle Valve", boardConfiguration->idle.solenoidPin, &enginePins.idleSolenoidPin,
				boardConfiguration->idle.solenoidFrequency, boardConfiguration->manIdlePosition / 100,
				applyIdleSolenoidPinState);
	}
}

void startIdleThread(Logging*sharedLogger) {
	logger = sharedLogger;

	// todo: re-initialize idle pins on the fly
	initIdleHardware();

	//scheduleMsg(logger, "initial idle %d", idlePositionController.value);

	chThdCreateStatic(ivThreadStack, sizeof(ivThreadStack), NORMALPRIO, (tfunc_t) ivThread, NULL);

	// this is idle switch INPUT - sometimes there is a switch on the throttle pedal
	// this switch is not used yet
	if (boardConfiguration->clutchDownPin != GPIO_UNASSIGNED) {
		efiSetPadMode("clutch down switch", boardConfiguration->clutchDownPin,
				getInputMode(boardConfiguration->clutchDownPinMode));
	}

	if (boardConfiguration->clutchUpPin != GPIO_UNASSIGNED) {
		efiSetPadMode("clutch up switch", boardConfiguration->clutchUpPin,
				getInputMode(boardConfiguration->clutchUpPinMode));
	}

	if (engineConfiguration->brakePedalPin != GPIO_UNASSIGNED) {
		efiSetPadMode("brake pedal switch", engineConfiguration->brakePedalPin,
				getInputMode(engineConfiguration->brakePedalPinMode));
	}

	addConsoleAction("idleinfo", showIdleInfo);

	addConsoleActionII("blipidle", blipIdle);

	// split this whole file into manual controller and auto controller? move these commands into the file
	// which would be dedicated to just auto-controller?

	addConsoleAction("idlebench", startIdleBench);
	apply();
}

#endif
