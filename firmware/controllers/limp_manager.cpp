#include "pch.h"

#include "limp_manager.h"
#include "fuel_math.h"
#include "main_trigger_callback.h"

#if EFI_ENGINE_CONTROL

#define CLEANUP_MODE_TPS 90

static bool noFiringUntilVvtSync(vvt_mode_e vvtMode) {
	auto operationMode = getEngineRotationState()->getOperationMode();

	// V-Twin MAP phase sense needs to always wait for sync
	if (vvtMode == VVT_MAP_V_TWIN) {
		return true;
	}

	if (engineConfiguration->isPhaseSyncRequiredForIgnition) {
		// in rare cases engines do not like random sequential mode
		return true;
	}
	if (isGdiEngine()) {
#if EFI_PROD_CODE
	    criticalError("For GDI please configure CAM and require sync for ignition");
#endif
	}

	// Odd cylinder count engines don't work properly with wasted spark, so wait for full sync (so that sequential works)
	// See https://github.com/rusefi/rusefi/issues/4195 for the issue to properly support this case
	if (engineConfiguration->cylindersCount > 1 && engineConfiguration->cylindersCount % 2 == 1) {
		return true;
	}

	// Symmetrical crank modes require cam sync before firing
	// non-symmetrical cranks can use faster spin-up mode (firing in wasted/batch before VVT sync)
	// Examples include Nissan MR/VQ, Miata NB, etc
	return
		operationMode == FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR ||
		operationMode == FOUR_STROKE_THREE_TIMES_CRANK_SENSOR ||
		operationMode == FOUR_STROKE_TWELVE_TIMES_CRANK_SENSOR;
}

void LimpManager::onFastCallback() {
	updateState(Sensor::getOrZero(SensorType::Rpm), getTimeNowNt());
}

void LimpManager::updateRevLimit(int rpm) {
	// User-configured hard RPM limit, either constant or CLT-lookup
	m_revLimit = engineConfiguration->useCltBasedRpmLimit
		? interpolate2d(Sensor::getOrZero(SensorType::Clt), engineConfiguration->cltRevLimitRpmBins, engineConfiguration->cltRevLimitRpm)
		: (float)engineConfiguration->rpmHardLimit;

	// Require configurable rpm drop before resuming
	resumeRpm = m_revLimit - engineConfiguration->rpmHardLimitHyst;

	m_timingRetard = interpolateClamped(resumeRpm, 0, m_revLimit, engineConfiguration->rpmSoftLimitTimingRetard, rpm);

	percent_t fuelAdded = interpolateClamped(resumeRpm, 0, m_revLimit, engineConfiguration->rpmSoftLimitFuelAdded, rpm);
	m_fuelCorrection = 1.0f + fuelAdded / 100;
}

void LimpManager::updateState(int rpm, efitick_t nowNt) {
	Clearable allowFuel = engineConfiguration->isInjectionEnabled;
	Clearable allowSpark = engineConfiguration->isIgnitionEnabled;

#if !EFI_UNIT_TEST
	if (!m_ignitionOn) {
		allowFuel.clear(ClearReason::IgnitionOff);
		allowSpark.clear(ClearReason::IgnitionOff);
	}
#endif

    if (isGdiEngine()) {
        if (gdiComms.getElapsedSeconds() > 1) {
            allowFuel.clear(ClearReason::GdiComms);
        }
    }

	if (engine->engineState.lua.luaIgnCut) {
		allowSpark.clear(ClearReason::Lua);
	}

#if EFI_HD_ACR
	// Don't inject fuel during Harley compression release - it sprays fuel everywhere
	if (engine->module<HarleyAcr>()->isActive()) {
		allowFuel.clear(ClearReason::ACR);
	}
#endif // EFI_HD_ACR

	updateRevLimit(rpm);
	if (m_revLimitHysteresis.test(rpm, m_revLimit, resumeRpm)) {
		if (engineConfiguration->cutFuelOnHardLimit) {
			allowFuel.clear(ClearReason::HardLimit);
		}

		if (engineConfiguration->cutSparkOnHardLimit) {
			allowSpark.clear(ClearReason::HardLimit);
		}
	}

	if (engine->lambdaMonitor.isCut()) {
		allowFuel.clear(ClearReason::LambdaProtection);
	}

	if (noFiringUntilVvtSync(engineConfiguration->vvtMode[0])
			&& !engine->triggerCentral.triggerState.hasSynchronizedPhase()) {
		// Any engine that requires cam-assistance for a full crank sync (symmetrical crank) can't schedule until we have cam sync
		// examples:
		// NB2, Nissan VQ/MR: symmetrical crank wheel and we need to make sure no spark happens out of sync
		// VTwin Harley: uneven firing order, so we need "cam" MAP sync to make sure no spark happens out of sync
		allowFuel.clear(ClearReason::EnginePhase);
		allowSpark.clear(ClearReason::EnginePhase);
	}

	// Force fuel limiting on the fault rev limit
	if (rpm > m_faultRevLimit) {
		allowFuel.clear(ClearReason::FaultRevLimit);
	}

	// Limit fuel only on boost pressure (limiting spark bends valves)
	float mapCut = engineConfiguration->boostCutPressure;
	if (mapCut != 0) {
		// require drop of 'boostCutPressureHyst' kPa to resume fuel
		if (m_boostCutHysteresis.test(Sensor::getOrZero(SensorType::Map), mapCut, mapCut - engineConfiguration->boostCutPressureHyst)) {
			allowFuel.clear(ClearReason::BoostCut);
		}
	}

	if (engine->rpmCalculator.isRunning()) {
		uint16_t minOilPressure = engineConfiguration->minOilPressureAfterStart;

		// Only check if the setting is enabled and you have an oil pressure sensor
		if (minOilPressure > 0 && Sensor::hasSensor(SensorType::OilPressure)) {
			// Has it been long enough we should have pressure?
			bool isTimedOut = engine->rpmCalculator.getSecondsSinceEngineStart(nowNt) > 5.0f;

			// Only check before timed out
			if (!isTimedOut) {
				auto oilp = Sensor::get(SensorType::OilPressure);

				if (oilp) {
					// We had oil pressure! Set the flag.
					if (oilp.Value > minOilPressure) {
						m_hadOilPressureAfterStart = true;
					}
				}
			}

			// If time is up, the sensor works, and no pressure, kill the engine.
			if (isTimedOut && !m_hadOilPressureAfterStart) {
				allowFuel.clear(ClearReason::OilPressure);
			}
		}
	} else {
		// reset state in case of stalled engine
		m_hadOilPressureAfterStart = false;
	}

	// If we're in engine stop mode, inhibit fuel
	if (shutdownController.isEngineStop(nowNt)) {
		/**
		 * todo: we need explicit clarification on why do we cut fuel but do not cut spark here!
		 */
		allowFuel.clear(ClearReason::StopRequested);
	}

	// If duty cycle is high, impose a fuel cut rev limiter.
	// This is safer than attempting to limp along with injectors or a pump that are out of flow.
	// only reset once below 20% duty to force the driver to lift
	float maxAllowedDuty = isGdiEngine() ? 50 : 96; // at the moment GDI means GDI4 and PT2001 which means duty cycle limited at 50% by hardware
	if (m_injectorDutyCutHysteresis.test(getInjectorDutyCycle(rpm), maxAllowedDuty, 20)) {
		allowFuel.clear(ClearReason::InjectorDutyCycle);
	}

	// If the pedal is pushed while not running, cut fuel to clear a flood condition.
	if (!engine->rpmCalculator.isRunning() &&
		engineConfiguration->isCylinderCleanupEnabled &&
		Sensor::getOrZero(SensorType::DriverThrottleIntent) > CLEANUP_MODE_TPS) {
		allowFuel.clear(ClearReason::FloodClear);
	}

	if (!engine->isMainRelayEnabled()) {
/*
todo AndreiKA this change breaks 22 unit tests?
		allowFuel.clear();
		allowSpark.clear();
*/
	}
	
#if EFI_LAUNCH_CONTROL
	// Fuel cut if launch control engaged
	if (engine->launchController.isLaunchFuelRpmRetardCondition()) {
		allowFuel.clear(ClearReason::LaunchCut);
	}

	// Spark cut if launch control engaged
	if (engine->launchController.isLaunchSparkRpmRetardCondition()) {
		allowSpark.clear(ClearReason::LaunchCut);
	}
#endif // EFI_LAUNCH_CONTROL

	m_transientAllowInjection = allowFuel;
	m_transientAllowIgnition = allowSpark;

	if (!m_transientAllowInjection || !m_transientAllowIgnition) {
		// Tracks the last time any cut happened
		m_lastCutTime.reset(nowNt);
	}
}

void LimpManager::onIgnitionStateChanged(bool ignitionOn) {
	m_ignitionOn = ignitionOn;
}

void LimpManager::reportEtbProblem() {
	m_allowEtb.clear(ClearReason::EtbProblem);
	setFaultRevLimit(/*rpm*/1500);
}

void LimpManager::fatalError() {
	m_allowEtb.clear(ClearReason::Fatal);
	m_allowIgnition.clear(ClearReason::Fatal);
	m_allowInjection.clear(ClearReason::Fatal);
	m_allowTriggerInput.clear(ClearReason::Fatal);

	setFaultRevLimit(/*rpm*/0);
}

void LimpManager::setFaultRevLimit(int limit) {
	// Only allow decreasing the limit
	// aka uses the limit of the worst fault to yet occur
	m_faultRevLimit = minI(m_faultRevLimit, limit);
}

bool LimpManager::allowElectronicThrottle() const {
	return m_allowEtb;
}

bool LimpManager::allowTriggerInput() const {
	return m_allowTriggerInput;
}

LimpState LimpManager::allowInjection() const {
	if (!m_allowInjection) {
		return {false, m_allowInjection.clearReason};
	}
	if (!m_transientAllowInjection) {
		return {false, m_transientAllowInjection.clearReason};
	}
	return {true, ClearReason::None};
}

LimpState LimpManager::allowIgnition() const {
	if (!m_allowIgnition) {
		return {false, m_allowIgnition.clearReason};
	}
	if (!m_transientAllowIgnition) {
		return {false, m_transientAllowIgnition.clearReason};
	}
	return {true, ClearReason::None};
}

angle_t LimpManager::getLimitingTimingRetard() const {
	if (!engineConfiguration->cutSparkOnHardLimit)
		return 0;
	return m_timingRetard;
}

float LimpManager::getLimitingFuelCorrection() const {
	if (!engineConfiguration->cutFuelOnHardLimit)
		return 1.0f;	// no correction
	return m_fuelCorrection;
}

float LimpManager::getTimeSinceAnyCut() const {
	return m_lastCutTime.getElapsedSeconds();
}
#endif // EFI_ENGINE_CONTROL
