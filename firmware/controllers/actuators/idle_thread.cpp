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


#if EFI_IDLE_CONTROL
#include "rpm_calculator.h"
#include "idle_thread.h"
#include "idle_hardware.h"

#include "periodic_task.h"
#include "allsensors.h"
#include "vehicle_speed.h"
#include "dc_motors.h"

#if EFI_TUNER_STUDIO
#include "stepper.h"
#endif

// todo: move all static vars to engine->engineState.idle?

static bool shouldResetPid = false;
// The idea of 'mightResetPid' is to reset PID only once - each time when TPS > idlePidDeactivationTpsThreshold.
// The throttle pedal can be pressed for a long time, making the PID data obsolete (thus the reset is required).
// We set 'mightResetPid' to true only if PID was actually used (i.e. idlePid.getOutput() was called) to save some CPU resources.
// See automaticIdleController().
static bool mightResetPid = false;

// This is needed to slowly turn on the PID back after it was reset.
static bool wasResetPid = false;
// This is used when the PID configuration is changed, to guarantee the reset
static bool mustResetPid = false;
static efitimeus_t restoreAfterPidResetTimeUs = 0;


class PidWithOverrides : public PidIndustrial {
public:
	float getOffset() const override {
#if EFI_UNIT_TEST
	EXPAND_Engine;
#endif
		float result = parameters->offset;
#if EFI_FSIO
			if (engineConfiguration->useFSIO12ForIdleOffset) {
				return result + ENGINE(fsioState.fsioIdleOffset);
			}
#endif /* EFI_FSIO */
		return result;
	}

	float getMinValue() const override {
#if EFI_UNIT_TEST
	EXPAND_Engine;
#endif
	float result = parameters->minValue;
#if EFI_FSIO
			if (engineConfiguration->useFSIO13ForIdleMinValue) {
				return result + ENGINE(fsioState.fsioIdleMinValue);
			}
#endif /* EFI_FSIO */
		return result;
	}
};

static PidWithOverrides industrialWithOverrideIdlePid;

#if EFI_IDLE_PID_CIC
// Use PID with CIC integrator
static PidCic idleCicPid;
#endif //EFI_IDLE_PID_CIC

Pid * getIdlePid(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if EFI_IDLE_PID_CIC
	if (CONFIG(useCicPidForIdle)) {
		return &idleCicPid;
	}
#endif /* EFI_IDLE_PID_CIC */
	return &industrialWithOverrideIdlePid;
}

float getIdlePidOffset(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return getIdlePid(PASS_ENGINE_PARAMETER_SIGNATURE)->getOffset();
}

float getIdlePidMinValue(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return getIdlePid(PASS_ENGINE_PARAMETER_SIGNATURE)->getMinValue();
}

static uint32_t lastCrankingCyclesCounter = 0;
static float lastCrankingIacPosition;

static iacPidMultiplier_t iacPidMultMap;

#if ! EFI_UNIT_TEST

void idleDebug(const char *msg, percent_t value) {
	efiPrintf("idle debug: %s%.2f", msg, value);
}

static void showIdleInfo(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	const char * idleModeStr = getIdle_mode_e(engineConfiguration->idleMode);
	efiPrintf("useStepperIdle=%s useHbridges=%s",
			boolToString(CONFIG(useStepperIdle)), boolToString(CONFIG(useHbridges)));
	efiPrintf("idleMode=%s position=%.2f",
			idleModeStr, getIdlePosition());

	if (CONFIG(useStepperIdle)) {
		if (CONFIG(useHbridges)) {
			efiPrintf("Coil A:");
			efiPrintf(" pin1=%s", hwPortname(CONFIG(stepperDcIo[0].directionPin1)));
			efiPrintf(" pin2=%s", hwPortname(CONFIG(stepperDcIo[0].directionPin2)));
			showDcMotorInfo(2);
			efiPrintf("Coil B:");
			efiPrintf(" pin1=%s", hwPortname(CONFIG(stepperDcIo[1].directionPin1)));
			efiPrintf(" pin2=%s", hwPortname(CONFIG(stepperDcIo[1].directionPin2)));
			showDcMotorInfo(3);
		} else {
			efiPrintf("directionPin=%s reactionTime=%.2f", hwPortname(CONFIG(idle).stepperDirectionPin),
					engineConfiguration->idleStepperReactionTime);
			efiPrintf("stepPin=%s steps=%d", hwPortname(CONFIG(idle).stepperStepPin),
					engineConfiguration->idleStepperTotalSteps);
			efiPrintf("enablePin=%s/%d", hwPortname(engineConfiguration->stepperEnablePin),
					engineConfiguration->stepperEnablePinMode);
		}
	} else {
		if (!CONFIG(isDoubleSolenoidIdle)) {
			efiPrintf("idle valve freq=%d on %s", CONFIG(idle).solenoidFrequency,
					hwPortname(CONFIG(idle).solenoidPin));
		} else {
			efiPrintf("idle valve freq=%d on %s", CONFIG(idle).solenoidFrequency,
					hwPortname(CONFIG(idle).solenoidPin));
			efiPrintf(" and %s", hwPortname(CONFIG(secondSolenoidPin)));
		}
	}

	if (engineConfiguration->idleMode == IM_AUTO) {
		getIdlePid(PASS_ENGINE_PARAMETER_SIGNATURE)->showPidStatus("idle");
	}
}

void setIdleMode(idle_mode_e value DECLARE_ENGINE_PARAMETER_SUFFIX) {
	engineConfiguration->idleMode = value ? IM_AUTO : IM_MANUAL;
	showIdleInfo();
}

percent_t getIdlePosition() {
	return engine->engineState.idle.currentIdlePosition;
}

void setManualIdleValvePosition(int positionPercent) {
	if (positionPercent < 1 || positionPercent > 99)
		return;
	efiPrintf("setting idle valve position %d", positionPercent);
#if ! EFI_UNIT_TEST
	showIdleInfo();
#endif /* EFI_UNIT_TEST */
	// todo: this is not great that we have to write into configuration here
	CONFIG(manIdlePosition) = positionPercent;
}

#endif /* EFI_UNIT_TEST */

void IdleController::init(pid_s* idlePidConfig) {
	m_timingPid.initPidClass(idlePidConfig);
}

int IdleController::getTargetRpm(float clt) const {
	auto target = interpolate2d(clt, CONFIG(cltIdleRpmBins), CONFIG(cltIdleRpm));

	// Bump for AC
	target += engine->acSwitchState ? CONFIG(acIdleRpmBump) : 0;

	// Bump by FSIO
	target += engine->fsioState.fsioIdleTargetRPMAdjustment;

	return target;
}

IIdleController::Phase IdleController::determinePhase(int rpm, int targetRpm, SensorResult tps, float vss, float crankingTaperFraction) const {
	if (!engine->rpmCalculator.isRunning()) {
		return Phase::Cranking;
	}

	if (!tps) {
		// If the TPS has failed, assume the engine is running
		return Phase::Running;
	}

	// if throttle pressed, we're out of the idle corner
	if (tps.Value > CONFIG(idlePidDeactivationTpsThreshold)) {
		return Phase::Running;
	}

	// If rpm too high (but throttle not pressed), we're coasting
	int maximumIdleRpm = targetRpm + CONFIG(idlePidRpmUpperLimit);
	if (rpm > maximumIdleRpm) {
		return Phase::Coasting;
	}

	// If the vehicle is moving too quickly, disable CL idle
	auto maxVss = CONFIG(maxIdleVss);
	if (maxVss != 0 && vss > maxVss) {
		return Phase::Running;
	}

	// If still in the cranking taper, disable closed loop idle
	if (crankingTaperFraction < 1) {
		return Phase::CrankToRunTaper;
	}

	// No other conditions met, we are idling!
	return Phase::Idling;
}

float IdleController::getCrankingTaperFraction() const {
	return (float)engine->rpmCalculator.getRevolutionCounterSinceStart() / CONFIG(afterCrankingIACtaperDuration);
}

float IdleController::getCrankingOpenLoop(float clt) const {
	float mult =
		CONFIG(overrideCrankingIacSetting)
		// Override to separate table
	 	? interpolate2d(clt, config->cltCrankingCorrBins, config->cltCrankingCorr)
		// Otherwise use plain running table
		: interpolate2d(clt, config->cltIdleCorrBins, config->cltIdleCorr);

	return CONFIG(crankingIACposition) * mult;
}

float IdleController::getRunningOpenLoop(float clt, SensorResult tps) const {
	float running =
		CONFIG(manIdlePosition)		// Base idle position (slider)
		* interpolate2d(clt, config->cltIdleCorrBins, config->cltIdleCorr);

	// Now we bump it by the AC/fan amount if necessary
	running += engine->acSwitchState ? CONFIG(acIdleExtraOffset) : 0;
	running += enginePins.fanRelay.getLogicValue() ? CONFIG(fan1ExtraIdle) : 0;
	running += enginePins.fanRelay2.getLogicValue() ? CONFIG(fan2ExtraIdle) : 0;

	// Now bump it by the specified amount when the throttle is opened (if configured)
	// nb: invalid tps will make no change, no explicit check required
	running += interpolateClamped(
		0, 0,
		CONFIG(idlePidDeactivationTpsThreshold), CONFIG(iacByTpsTaper),
		tps.value_or(0));

	return clampF(0, running, 100);
}

float IdleController::getOpenLoop(Phase phase, float clt, SensorResult tps, float crankingTaperFraction) const {
	float cranking = getCrankingOpenLoop(clt);

	// if we're cranking, nothing more to do.
	if (phase == Phase::Cranking) {
		return cranking;
	}

	float running = getRunningOpenLoop(clt, tps);

	if (phase == Phase::CrankToRunTaper) {
		// Interpolate between cranking and running over a short time
		// This clamps once you fall off the end, so no explicit check for >1 required
		return interpolateClamped(0, cranking, 1, running, crankingTaperFraction);
	}

	// If coasting (and enabled), use the coasting position table instead of normal open loop
	// TODO: this should be a table of open loop mult vs. RPM, not vs. clt
	if (CONFIG(useIacTableForCoasting) && phase == Phase::Coasting) {
		return interpolate2d(clt, CONFIG(iacCoastingBins), CONFIG(iacCoasting));
	}

	return running;
}

float IdleController::getIdleTimingAdjustment(int rpm) {
	return getIdleTimingAdjustment(rpm, m_lastTargetRpm, m_lastPhase);
}

float IdleController::getIdleTimingAdjustment(int rpm, int targetRpm, Phase phase) {
	// if not enabled, do nothing
	if (!CONFIG(useIdleTimingPidControl)) {
		return 0;
	}

	// If not idling, do nothing
	if (phase != Phase::Idling) {
		m_timingPid.reset();
		return 0;
	}

	if (CONFIG(useInstantRpmForIdle)) {
		rpm = engine->triggerCentral.triggerState.getInstantRpm();
	}

	// If inside the deadzone, do nothing
	if (absI(rpm - targetRpm) < CONFIG(idleTimingPidDeadZone)) {
		m_timingPid.reset();
		return 0;
	}

	// We're now in the idle mode, and RPM is inside the Timing-PID regulator work zone!
	return m_timingPid.getOutput(targetRpm, rpm, FAST_CALLBACK_PERIOD_MS / 1000.0f);
}

/**
 * idle blip is a development tool: alternator PID research for instance have benefited from a repetitive change of RPM
 */
static percent_t blipIdlePosition;
static efitimeus_t timeToStopBlip = 0;
efitimeus_t timeToStopIdleTest = 0;

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

/**
 * @return idle valve position percentage for automatic closed loop mode
 */
float IdleController::getClosedLoop(IIdleController::Phase phase, float tpsPos, int rpm, int targetRpm) {
	auto idlePid = getIdlePid(PASS_ENGINE_PARAMETER_SIGNATURE);

	if (shouldResetPid) {
		// we reset only if I-term is negative, because the positive I-term is good - it keeps RPM from dropping too low
		if (idlePid->getIntegration() <= 0 || mustResetPid) {
			idlePid->reset();
			mustResetPid = false;
		}
//			alternatorPidResetCounter++;
		shouldResetPid = false;
		wasResetPid = true;
	}

	// todo: move this to pid_s one day
	industrialWithOverrideIdlePid.antiwindupFreq = engineConfiguration->idle_antiwindupFreq;
	industrialWithOverrideIdlePid.derivativeFilterLoss = engineConfiguration->idle_derivativeFilterLoss;

	efitimeus_t nowUs = getTimeNowUs();

	if (phase != IIdleController::Phase::Idling) {
		// Don't store old I and D terms if PID doesn't work anymore.
		// Otherwise they will affect the idle position much later, when the throttle is closed.
		if (mightResetPid) {
			mightResetPid = false;
			shouldResetPid = true;
		}

		engine->engineState.idle.idleState = TPS_THRESHOLD;

		// We aren't idling, so don't apply any correction.  A positive correction could inhibit a return to idle.
		m_lastAutomaticPosition = 0;
		return 0;
	}

	// #1553 we need to give FSIO variable offset or minValue a chance
	bool acToggleJustTouched = (nowUs - engine->acSwitchLastChangeTime) < MS2US(500);
	// check if within the dead zone
	if (!acToggleJustTouched && absI(rpm - targetRpm) <= CONFIG(idlePidRpmDeadZone)) {
		engine->engineState.idle.idleState = RPM_DEAD_ZONE;
		// current RPM is close enough, no need to change anything
		return m_lastAutomaticPosition;
	}

	// When rpm < targetRpm, there's a risk of dropping RPM too low - and the engine dies out.
	// So PID reaction should be increased by adding extra percent to PID-error:
	percent_t errorAmpCoef = 1.0f;
	if (rpm < targetRpm) {
		errorAmpCoef += (float)CONFIG(pidExtraForLowRpm) / PERCENT_MULT;
	}

	// if PID was previously reset, we store the time when it turned on back (see errorAmpCoef correction below)
	if (wasResetPid) {
		restoreAfterPidResetTimeUs = nowUs;
		wasResetPid = false;
	}
	// increase the errorAmpCoef slowly to restore the process correctly after the PID reset
	// todo: move restoreAfterPidResetTimeUs to engineState.idle?
	efitimeus_t timeSincePidResetUs = nowUs - /*engine->engineState.idle.*/restoreAfterPidResetTimeUs;
	// todo: add 'pidAfterResetDampingPeriodMs' setting
	errorAmpCoef = interpolateClamped(0, 0, MS2US(/*CONFIG(pidAfterResetDampingPeriodMs)*/1000), errorAmpCoef, timeSincePidResetUs);
	// If errorAmpCoef > 1.0, then PID thinks that RPM is lower than it is, and controls IAC more aggressively
	idlePid->setErrorAmplification(errorAmpCoef);

	percent_t newValue = idlePid->getOutput(targetRpm, rpm, SLOW_CALLBACK_PERIOD_MS / 1000.0f);
	engine->engineState.idle.idleState = PID_VALUE;

	// the state of PID has been changed, so we might reset it now, but only when needed (see idlePidDeactivationTpsThreshold)
	mightResetPid = true;

	// Apply PID Multiplier if used
	if (CONFIG(useIacPidMultTable)) {
		float engineLoad = getFuelingLoad(PASS_ENGINE_PARAMETER_SIGNATURE);
		float multCoef = iacPidMultMap.getValue(rpm / RPM_1_BYTE_PACKING_MULT, engineLoad);
		// PID can be completely disabled of multCoef==0, or it just works as usual if multCoef==1
		newValue = interpolateClamped(0, 0, 1, newValue, multCoef);
	}
	
	// Apply PID Deactivation Threshold as a smooth taper for TPS transients.
	// if tps==0 then PID just works as usual, or we completely disable it if tps>=threshold
	// TODO: should we just remove this? It reduces the gain if your zero throttle stop isn't perfect,
	// which could give unstable results.
	newValue = interpolateClamped(0, newValue, CONFIG(idlePidDeactivationTpsThreshold), 0, tpsPos);

	m_lastAutomaticPosition = newValue;
	return newValue;
}

	float IdleController::getIdlePosition() {
		// Simplify hardware CI: we borrow the idle valve controller as a PWM source for various stimulation tasks
		// The logic in this function is solidly unit tested, so it's not necessary to re-test the particulars on real hardware.
		#ifdef HARDWARE_CI
			return CONFIG(manIdlePosition);
		#endif

	/*
	 * Here we have idle logic thread - actual stepper movement is implemented in a separate
	 * working thread,
	 * @see stepper.cpp
	 */

		getIdlePid(PASS_ENGINE_PARAMETER_SIGNATURE)->iTermMin = engineConfiguration->idlerpmpid_iTermMin;
		getIdlePid(PASS_ENGINE_PARAMETER_SIGNATURE)->iTermMax = engineConfiguration->idlerpmpid_iTermMax;


		// On failed sensor, use 0 deg C - should give a safe highish idle
		float clt = Sensor::get(SensorType::Clt).value_or(0);
		auto tps = Sensor::get(SensorType::DriverThrottleIntent);

		float rpm;
		if (CONFIG(useInstantRpmForIdle)) {
			rpm = engine->triggerCentral.triggerState.getInstantRpm();
		} else {
			rpm = GET_RPM();
		}

		// Compute the target we're shooting for
		auto targetRpm = getTargetRpm(clt);
		m_lastTargetRpm = targetRpm;

		// Determine cranking taper
		float crankingTaper = getCrankingTaperFraction();

		// Determine what operation phase we're in - idling or not
		auto phase = determinePhase(rpm, targetRpm, tps, getVehicleSpeed(), crankingTaper);
		m_lastPhase = phase;

		engine->engineState.isAutomaticIdle = tps.Valid && engineConfiguration->idleMode == IM_AUTO;

		if (engineConfiguration->isVerboseIAC && engine->engineState.isAutomaticIdle) {
			efiPrintf("Idle state %s", getIdle_state_e(engine->engineState.idle.idleState));
			getIdlePid(PASS_ENGINE_PARAMETER_SIGNATURE)->showPidStatus("idle");
		}

		finishIdleTestIfNeeded();
		undoIdleBlipIfNeeded();

		percent_t iacPosition;

		if (timeToStopBlip != 0) {
			iacPosition = blipIdlePosition;
			engine->engineState.idle.idleState = BLIP;
		} else {
			// Always apply closed loop correction
			iacPosition = getOpenLoop(phase, clt, tps, crankingTaper);
			engine->engineState.idle.baseIdlePosition = iacPosition;

			// If TPS is working and automatic mode enabled, add any automatic correction
			if (tps.Valid && engineConfiguration->idleMode == IM_AUTO) {
				iacPosition += getClosedLoop(phase, tps.Value, rpm, targetRpm);
			}

			iacPosition = clampPercentValue(iacPosition);
		}

#if EFI_TUNER_STUDIO
		tsOutputChannels.isIdleClosedLoop = phase == Phase::Idling;
		tsOutputChannels.isIdleCoasting = phase == Phase::Coasting;

		if (engineConfiguration->debugMode == DBG_IDLE_CONTROL) {
			if (engineConfiguration->idleMode == IM_AUTO) {
				// see also tsOutputChannels->idlePosition
				getIdlePid(PASS_ENGINE_PARAMETER_SIGNATURE)->postState(&tsOutputChannels, 1000000);
				tsOutputChannels.debugIntField4 = engine->engineState.idle.idleState;
			} else {
				tsOutputChannels.debugFloatField1 = iacPosition;
				extern StepperMotor iacMotor;
				tsOutputChannels.debugIntField1 = iacMotor.getTargetPosition();
			}
		}
#endif /* EFI_TUNER_STUDIO */

		engine->engineState.idle.currentIdlePosition = iacPosition;

		return iacPosition;
}

void IdleController::update() {
	float position = getIdlePosition();
	applyIACposition(position PASS_ENGINE_PARAMETER_SUFFIX);
}

IdleController idleControllerInstance;

void updateIdleControl()
{
	idleControllerInstance.update();
}

float getIdleTimingAdjustment(int rpm) {
	return idleControllerInstance.getIdleTimingAdjustment(rpm);
}

bool isIdling() {
	return idleControllerInstance.isIdling();
}

static void applyPidSettings(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	getIdlePid(PASS_ENGINE_PARAMETER_SIGNATURE)->updateFactors(engineConfiguration->idleRpmPid.pFactor, engineConfiguration->idleRpmPid.iFactor, engineConfiguration->idleRpmPid.dFactor);
	iacPidMultMap.init(CONFIG(iacPidMultTable), CONFIG(iacPidMultLoadBins), CONFIG(iacPidMultRpmBins));
}

void setDefaultIdleParameters(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->idleRpmPid.pFactor = 0.1f;
	engineConfiguration->idleRpmPid.iFactor = 0.05f;
	engineConfiguration->idleRpmPid.dFactor = 0.0f;

	engineConfiguration->idlerpmpid_iTermMin = -20;
	engineConfiguration->idlerpmpid_iTermMax =  20;

	// Good starting point is 10 degrees per 100 rpm, aka 0.1 deg/rpm
	CONFIG(idleTimingPid).pFactor = 0.1f;
	CONFIG(idleTimingPid).iFactor = 0;
	CONFIG(idleTimingPid).dFactor = 0;

	// Allow +- 10 degrees adjustment
	CONFIG(idleTimingPid).minValue = -10;
	CONFIG(idleTimingPid).minValue = 10;

	// Idle region is target + 100 RPM
	CONFIG(idlePidRpmUpperLimit) = 100;
}

#if ! EFI_UNIT_TEST

void onConfigurationChangeIdleCallback(engine_configuration_s *previousConfiguration) {
	shouldResetPid = !getIdlePid(PASS_ENGINE_PARAMETER_SIGNATURE)->isSame(&previousConfiguration->idleRpmPid);
	mustResetPid = shouldResetPid;
}

void setTargetIdleRpm(int value) {
	setTargetRpmCurve(value PASS_ENGINE_PARAMETER_SUFFIX);
	efiPrintf("target idle RPM %d", value);
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

/**
 * Idle test would activate the solenoid for three seconds
 */
void startIdleBench(void) {
	timeToStopIdleTest = getTimeNowUs() + MS2US(3000); // 3 seconds
	efiPrintf("idle valve bench test");
	showIdleInfo();
}

#endif /* EFI_UNIT_TEST */

void startIdleThread(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	INJECT_ENGINE_REFERENCE(&idleControllerInstance);
	idleControllerInstance.init(&CONFIG(idleTimingPid));
	INJECT_ENGINE_REFERENCE(&industrialWithOverrideIdlePid);

	ENGINE(idleController) = &idleControllerInstance;

	getIdlePid(PASS_ENGINE_PARAMETER_SIGNATURE)->initPidClass(&engineConfiguration->idleRpmPid);

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

#if EFI_PROD_CODE
	// this is neutral/no gear switch input. on Miata it's wired both to clutch pedal and neutral in gearbox
	// this switch is not used yet
	if (isBrainPinValid(CONFIG(clutchDownPin))) {
		efiSetPadMode("clutch down switch", CONFIG(clutchDownPin),
				getInputMode(CONFIG(clutchDownPinMode)));
	}

	if (isBrainPinValid(CONFIG(clutchUpPin))) {
		efiSetPadMode("clutch up switch", CONFIG(clutchUpPin),
				getInputMode(CONFIG(clutchUpPinMode)));
	}

	if (isBrainPinValid(CONFIG(throttlePedalUpPin))) {
		efiSetPadMode("throttle pedal up switch", CONFIG(throttlePedalUpPin),
				getInputMode(CONFIG(throttlePedalUpPinMode)));
	}

	if (isBrainPinValid(engineConfiguration->brakePedalPin)) {
		efiSetPadMode("brake pedal switch", engineConfiguration->brakePedalPin,
				getInputMode(engineConfiguration->brakePedalPinMode));
	}
#endif /* EFI_PROD_CODE */

#if ! EFI_UNIT_TEST

	addConsoleAction("idleinfo", showIdleInfo);

	addConsoleActionII("blipidle", blipIdle);

	// split this whole file into manual controller and auto controller? move these commands into the file
	// which would be dedicated to just auto-controller?

	addConsoleAction("idlebench", startIdleBench);
#endif /* EFI_UNIT_TEST */
	applyPidSettings(PASS_ENGINE_PARAMETER_SIGNATURE);
}

#endif /* EFI_IDLE_CONTROL */
