/**
 * @file    idle_thread.cpp
 * @brief   Idle Air Control valve thread.
 *
 * This thread looks at current RPM and decides if it should increase or decrease IAC duty cycle.
 * This file has the hardware & scheduling logic, desired idle level lives separately.
 *
 *
 * @date May 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2022
 */

#include "pch.h"

#if EFI_IDLE_CONTROL
#include "idle_thread.h"
#include "idle_hardware.h"

#include "periodic_task.h"
#include "dc_motors.h"

#if EFI_TUNER_STUDIO
#include "stepper.h"
#endif

int IdleController::getTargetRpm(float clt) {
	targetRpmByClt = interpolate2d(clt, config->cltIdleRpmBins, config->cltIdleRpm);

	// Bump for AC
	targetRpmAcBump = engine->module<AcController>().unmock().acButtonState ? engineConfiguration->acIdleRpmBump : 0;

	return targetRpmByClt + targetRpmAcBump;
}

IIdleController::Phase IdleController::determinePhase(int rpm, int targetRpm, SensorResult tps, float vss, float crankingTaperFraction) {
#if EFI_SHAFT_POSITION_INPUT
	if (!engine->rpmCalculator.isRunning()) {
		return Phase::Cranking;
	}
	badTps = !tps;

	if (badTps) {
		// If the TPS has failed, assume the engine is running
		return Phase::Running;
	}

	// if throttle pressed, we're out of the idle corner
	if (tps.Value > engineConfiguration->idlePidDeactivationTpsThreshold) {
		return Phase::Running;
	}

	// If rpm too high (but throttle not pressed), we're coasting
	int maximumIdleRpm = targetRpm + engineConfiguration->idlePidRpmUpperLimit;
	looksLikeCoasting = rpm > maximumIdleRpm;
	if (looksLikeCoasting) {
		return Phase::Coasting;
	}

	// If the vehicle is moving too quickly, disable CL idle
	auto maxVss = engineConfiguration->maxIdleVss;
	looksLikeRunning = maxVss != 0 && vss > maxVss;
	if (looksLikeRunning) {
		return Phase::Running;
	}

	// If still in the cranking taper, disable closed loop idle
	looksLikeCrankToIdle = crankingTaperFraction < 1;
	if (looksLikeCrankToIdle) {
		return Phase::CrankToIdleTaper;
	}
#endif // EFI_SHAFT_POSITION_INPUT

	// No other conditions met, we are idling!
	return Phase::Idling;
}

float IdleController::getCrankingTaperFraction() const {
	return (float)engine->rpmCalculator.getRevolutionCounterSinceStart() / engineConfiguration->afterCrankingIACtaperDuration;
}

float IdleController::getCrankingOpenLoop(float clt) const {
	float mult =
		engineConfiguration->overrideCrankingIacSetting
		// Override to separate table
	 	? interpolate2d(clt, config->cltCrankingCorrBins, config->cltCrankingCorr)
		// Otherwise use plain running table
		: interpolate2d(clt, config->cltIdleCorrBins, config->cltIdleCorr);

	return engineConfiguration->crankingIACposition * mult;
}

percent_t IdleController::getRunningOpenLoop(float clt, SensorResult tps) {
	float running =
		engineConfiguration->manIdlePosition		// Base idle position (slider)
		* interpolate2d(clt, config->cltIdleCorrBins, config->cltIdleCorr);

	// Now we bump it by the AC/fan amount if necessary
	running += engine->module<AcController>().unmock().acButtonState ? engineConfiguration->acIdleExtraOffset : 0;
	running += enginePins.fanRelay.getLogicValue() ? engineConfiguration->fan1ExtraIdle : 0;
	running += enginePins.fanRelay2.getLogicValue() ? engineConfiguration->fan2ExtraIdle : 0;

	running += luaAdd;

	// Now bump it by the specified amount when the throttle is opened (if configured)
	// nb: invalid tps will make no change, no explicit check required
	iacByTpsTaper = interpolateClamped(
		0, 0,
		engineConfiguration->idlePidDeactivationTpsThreshold, engineConfiguration->iacByTpsTaper,
		tps.value_or(0));

	running += iacByTpsTaper;

	return clampF(0, running, 100);
}

percent_t IdleController::getOpenLoop(Phase phase, float clt, SensorResult tps, float crankingTaperFraction) {
	percent_t crankingValvePosition = getCrankingOpenLoop(clt);

	isCoasting = phase == Phase::Cranking;
	// if we're cranking, nothing more to do.
	if (isCoasting) {
		return crankingValvePosition;
	}

	// If coasting (and enabled), use the coasting position table instead of normal open loop
	// TODO: this should be a table of open loop mult vs. RPM, not vs. clt
	useIacTableForCoasting = engineConfiguration->useIacTableForCoasting && phase == Phase::Coasting;
	if (useIacTableForCoasting) {
		return interpolate2d(clt, config->iacCoastingBins, config->iacCoasting);
	}

	percent_t running = getRunningOpenLoop(clt, tps);

	// Interpolate between cranking and running over a short time
	// This clamps once you fall off the end, so no explicit check for >1 required
	return interpolateClamped(0, crankingValvePosition, 1, running, crankingTaperFraction);
}

float IdleController::getIdleTimingAdjustment(int rpm) {
	return getIdleTimingAdjustment(rpm, m_lastTargetRpm, m_lastPhase);
}

float IdleController::getIdleTimingAdjustment(int rpm, int targetRpm, Phase phase) {
	// if not enabled, do nothing
	if (!engineConfiguration->useIdleTimingPidControl) {
		return 0;
	}

	// If not idling, do nothing
	if (phase != Phase::Idling) {
		m_timingPid.reset();
		return 0;
	}
#if EFI_SHAFT_POSITION_INPUT
	if (engineConfiguration->useInstantRpmForIdle) {
		rpm = engine->triggerCentral.triggerState.getInstantRpm();
	}
#endif // EFI_SHAFT_POSITION_INPUT

	// If inside the deadzone, do nothing
	if (absI(rpm - targetRpm) < engineConfiguration->idleTimingPidDeadZone) {
		m_timingPid.reset();
		return 0;
	}

	// We're now in the idle mode, and RPM is inside the Timing-PID regulator work zone!
	return m_timingPid.getOutput(targetRpm, rpm, FAST_CALLBACK_PERIOD_MS / 1000.0f);
}

static void finishIdleTestIfNeeded() {
	if (engine->timeToStopIdleTest != 0 && getTimeNowUs() > engine->timeToStopIdleTest)
		engine->timeToStopIdleTest = 0;
}

static void undoIdleBlipIfNeeded() {
	if (engine->timeToStopBlip != 0 && getTimeNowUs() > engine->timeToStopBlip) {
		engine->timeToStopBlip = 0;
	}
}

/**
 * @return idle valve position percentage for automatic closed loop mode
 */
float IdleController::getClosedLoop(IIdleController::Phase phase, float tpsPos, int rpm, int targetRpm) {
	auto idlePid = getIdlePid();

	if (shouldResetPid) {
		needReset = idlePid->getIntegration() <= 0 || mustResetPid;
		// we reset only if I-term is negative, because the positive I-term is good - it keeps RPM from dropping too low
		if (needReset) {
			idlePid->reset();
			mustResetPid = false;
		}
		shouldResetPid = false;
		wasResetPid = true;
	}

	// todo: move this to pid_s one day
	industrialWithOverrideIdlePid.antiwindupFreq = engineConfiguration->idle_antiwindupFreq;
	industrialWithOverrideIdlePid.derivativeFilterLoss = engineConfiguration->idle_derivativeFilterLoss;

	efitimeus_t nowUs = getTimeNowUs();

	notIdling = phase != IIdleController::Phase::Idling;
	if (notIdling) {
		// Don't store old I and D terms if PID doesn't work anymore.
		// Otherwise they will affect the idle position much later, when the throttle is closed.
		if (mightResetPid) {
			mightResetPid = false;
			shouldResetPid = true;
		}

		idleState = TPS_THRESHOLD;

		// We aren't idling, so don't apply any correction.  A positive correction could inhibit a return to idle.
		m_lastAutomaticPosition = 0;
		return 0;
	}

	// #1553 we need to give FSIO variable offset or minValue a chance
	bool acToggleJustTouched = (US2MS(nowUs) - engine->module<AcController>().unmock().acSwitchLastChangeTimeMs) < 500/*ms*/;
	// check if within the dead zone
	isInDeadZone = !acToggleJustTouched && absI(rpm - targetRpm) <= engineConfiguration->idlePidRpmDeadZone;
	if (isInDeadZone) {
		idleState = RPM_DEAD_ZONE;
		// current RPM is close enough, no need to change anything
		return m_lastAutomaticPosition;
	}

	// When rpm < targetRpm, there's a risk of dropping RPM too low - and the engine dies out.
	// So PID reaction should be increased by adding extra percent to PID-error:
	percent_t errorAmpCoef = 1.0f;
	if (rpm < targetRpm) {
		errorAmpCoef += (float)engineConfiguration->pidExtraForLowRpm / PERCENT_MULT;
	}

	// if PID was previously reset, we store the time when it turned on back (see errorAmpCoef correction below)
	if (wasResetPid) {
		restoreAfterPidResetTimeUs = nowUs;
		wasResetPid = false;
	}
	// increase the errorAmpCoef slowly to restore the process correctly after the PID reset
	// todo: move restoreAfterPidResetTimeUs to idle?
	efitimeus_t timeSincePidResetUs = nowUs - restoreAfterPidResetTimeUs;
	// todo: add 'pidAfterResetDampingPeriodMs' setting
	errorAmpCoef = interpolateClamped(0, 0, MS2US(/*engineConfiguration->pidAfterResetDampingPeriodMs*/1000), errorAmpCoef, timeSincePidResetUs);
	// If errorAmpCoef > 1.0, then PID thinks that RPM is lower than it is, and controls IAC more aggressively
	idlePid->setErrorAmplification(errorAmpCoef);

	percent_t newValue = idlePid->getOutput(targetRpm, rpm, SLOW_CALLBACK_PERIOD_MS / 1000.0f);
	idleState = PID_VALUE;

	// the state of PID has been changed, so we might reset it now, but only when needed (see idlePidDeactivationTpsThreshold)
	mightResetPid = true;

	// Apply PID Multiplier if used
	if (engineConfiguration->useIacPidMultTable) {
		float engineLoad = getFuelingLoad();
		float multCoef = interpolate3d(
			config->iacPidMultTable,
			config->iacPidMultLoadBins, engineLoad,
			config->iacPidMultRpmBins, rpm
		);
		// PID can be completely disabled of multCoef==0, or it just works as usual if multCoef==1
		newValue = interpolateClamped(0, 0, 1, newValue, multCoef);
	}
	
	// Apply PID Deactivation Threshold as a smooth taper for TPS transients.
	// if tps==0 then PID just works as usual, or we completely disable it if tps>=threshold
	// TODO: should we just remove this? It reduces the gain if your zero throttle stop isn't perfect,
	// which could give unstable results.
	newValue = interpolateClamped(0, newValue, engineConfiguration->idlePidDeactivationTpsThreshold, 0, tpsPos);

	m_lastAutomaticPosition = newValue;
	return newValue;
}

float IdleController::getIdlePosition() {
#if EFI_SHAFT_POSITION_INPUT

		// Simplify hardware CI: we borrow the idle valve controller as a PWM source for various stimulation tasks
		// The logic in this function is solidly unit tested, so it's not necessary to re-test the particulars on real hardware.
		#ifdef HARDWARE_CI
			return engineConfiguration->manIdlePosition;
		#endif

	/*
	 * Here we have idle logic thread - actual stepper movement is implemented in a separate
	 * working thread see stepper.cpp
	 */
		getIdlePid()->iTermMin = engineConfiguration->idlerpmpid_iTermMin;
		getIdlePid()->iTermMax = engineConfiguration->idlerpmpid_iTermMax;

		// On failed sensor, use 0 deg C - should give a safe highish idle
		float clt = Sensor::getOrZero(SensorType::Clt);
		auto tps = Sensor::get(SensorType::DriverThrottleIntent);

        // this variable is here to make Live View happier
        useInstantRpmForIdle = engineConfiguration->useInstantRpmForIdle;
		float rpm;
		if (useInstantRpmForIdle) {
			rpm = engine->triggerCentral.triggerState.getInstantRpm();
		} else {
			rpm = Sensor::getOrZero(SensorType::Rpm);
		}

		// Compute the target we're shooting for
		auto targetRpm = getTargetRpm(clt);
		m_lastTargetRpm = targetRpm;

		// Determine cranking taper
		float crankingTaper = getCrankingTaperFraction();

		// Determine what operation phase we're in - idling or not
		float vehicleSpeed = Sensor::getOrZero(SensorType::VehicleSpeed);
		auto phase = determinePhase(rpm, targetRpm, tps, vehicleSpeed, crankingTaper);
		m_lastPhase = phase;

		bool isAutomaticIdle = tps.Valid && engineConfiguration->idleMode == IM_AUTO;

        isVerboseIAC = engineConfiguration->isVerboseIAC && isAutomaticIdle;
		if (isVerboseIAC) {
			efiPrintf("Idle state %s", getIdle_state_e(idleState));
			getIdlePid()->showPidStatus("idle");
		}

		finishIdleTestIfNeeded();
		undoIdleBlipIfNeeded();

		percent_t iacPosition;

		isBlipping = engine->timeToStopBlip != 0;
		if (isBlipping) {
			iacPosition = engine->blipIdlePosition;
			idleState = BLIP;
		} else {
			// Always apply closed loop correction
			iacPosition = getOpenLoop(phase, clt, tps, crankingTaper);
			baseIdlePosition = iacPosition;

			useClosedLoop = tps.Valid && engineConfiguration->idleMode == IM_AUTO;
			// If TPS is working and automatic mode enabled, add any automatic correction
			if (useClosedLoop) {
				iacPosition += getClosedLoop(phase, tps.Value, rpm, targetRpm);
			}

			iacPosition = clampPercentValue(iacPosition);
		}

#if EFI_TUNER_STUDIO && (EFI_PROD_CODE || EFI_SIMULATOR)
		engine->outputChannels.isIdleClosedLoop = phase == Phase::Idling;
		engine->outputChannels.isIdleCoasting = phase == Phase::Coasting;

			if (engineConfiguration->idleMode == IM_AUTO) {
				// see also tsOutputChannels->idlePosition
				getIdlePid()->postState(engine->outputChannels.idleStatus);
			} else {
				engine->outputChannels.idleCurrentPosition = iacPosition;
				extern StepperMotor iacMotor;
				engine->outputChannels.idleTargetPosition = iacMotor.getTargetPosition();
			}
#endif /* EFI_TUNER_STUDIO */

		currentIdlePosition = iacPosition;
		return iacPosition;
#else
		return 0;
#endif // EFI_SHAFT_POSITION_INPUT

}

void IdleController::onSlowCallback() {
	float position = getIdlePosition();
	applyIACposition(position);
}

void IdleController::onConfigurationChange(engine_configuration_s const * previousConfiguration) {
#if ! EFI_UNIT_TEST
	shouldResetPid = !getIdlePid()->isSame(&previousConfiguration->idleRpmPid);
	mustResetPid = shouldResetPid;
#endif
}

void IdleController::init() {
	shouldResetPid = false;
	mightResetPid = false;
	wasResetPid = false;
	m_timingPid.initPidClass(&engineConfiguration->idleTimingPid);
	getIdlePid()->initPidClass(&engineConfiguration->idleRpmPid);
}

#endif /* EFI_IDLE_CONTROL */
