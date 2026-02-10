#include "pch.h"

#include "limp_manager.h"
#include "fuel_math.h"
#include "main_trigger_callback.h"

#if EFI_ENGINE_CONTROL

#define CLEANUP_MODE_TPS 90

#if EFI_SHAFT_POSITION_INPUT
static bool noFiringUntilVvtSync() {
	auto operationMode = getEngineRotationState()->getOperationMode();

	if (engineConfiguration->isPhaseSyncRequiredForIgnition) {
		// in rare cases engines do not like random sequential mode
		return true;
	}
	if (isGdiEngine()) {
#if EFI_PROD_CODE
	    criticalError("For GDI please configure CAM and require sync for ignition");
#endif
	}

	if (engineConfiguration->ignitionMode == IM_ONE_COIL) {
	  // distributor routes to the correct cylinder, no need to worry about sync
	  return false;
	}

	// Symmetrical crank modes require cam sync before firing
	// non-symmetrical cranks can use faster spin-up mode (firing in wasted/batch before VVT sync)
	// Examples include Nissan MR/VQ, Miata NB, etc
	return
		operationMode == FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR ||
		operationMode == FOUR_STROKE_THREE_TIMES_CRANK_SENSOR ||
		operationMode == FOUR_STROKE_TWELVE_TIMES_CRANK_SENSOR;
}
#endif // EFI_SHAFT_POSITION_INPUT

void LimpManager::onFastCallback() {
	updateState(Sensor::getOrZero(SensorType::Rpm), getTimeNowNt());
}

void LimpManager::updateRevLimit(float rpm) {
	// User-configured hard RPM limit, either constant or CLT-lookup
	m_revLimit = engineConfiguration->useCltBasedRpmLimit
		? interpolate2d(Sensor::getOrZero(SensorType::Clt), config->cltRevLimitRpmBins, config->cltRevLimitRpm)
		: (float)engineConfiguration->rpmHardLimit;

	// Require configurable rpm drop before resuming
	resumeRpm = m_revLimit - engineConfiguration->rpmHardLimitHyst;

	m_timingRetard = interpolateClamped(resumeRpm, 0, m_revLimit, engineConfiguration->rpmSoftLimitTimingRetard, rpm);

	percent_t fuelAdded = interpolateClamped(resumeRpm, 0, m_revLimit, engineConfiguration->rpmSoftLimitFuelAdded, rpm);
	m_fuelCorrection = 1.0f + fuelAdded / 100;
}

void LimpManager::updateState(float rpm, efitick_t nowNt) {
	Clearable allowFuel = engineConfiguration->isInjectionEnabled;
	Clearable allowSpark = engineConfiguration->isIgnitionEnabled;

#if EFI_SHAFT_POSITION_INPUT && !EFI_UNIT_TEST
	if (!m_ignitionOn
	&& !engine->triggerCentral.directSelfStimulation // useful to try things on real ECU even without ignition voltage
	) {
		allowFuel.clear(ClearReason::IgnitionOff);
		allowSpark.clear(ClearReason::IgnitionOff);
	}
#endif

    if (isGdiEngine() && engineConfiguration->externalRusEfiGdiModule) {
        if (externalGdiCanBusComms.getElapsedSeconds() > 1) {
            allowFuel.clear(ClearReason::GdiComms);
        }
    }

	if (engine->engineState.lua.luaIgnCut) {
		allowSpark.clear(ClearReason::Lua);
	}

	if (engine->engineState.lua.luaFuelCut) {
		allowFuel.clear(ClearReason::Lua);
	}

	updateRevLimit(rpm);
	if (m_revLimitHysteresis.test(rpm, m_revLimit, resumeRpm)) {
		if (engineConfiguration->cutFuelOnHardLimit) {
			allowFuel.clear(ClearReason::HardLimit);
		}

		if (engineConfiguration->cutSparkOnHardLimit) {
			allowSpark.clear(ClearReason::HardLimit);
		}
	}

#if EFI_SHAFT_POSITION_INPUT
	if (engine->lambdaMonitor.isCut()) {
		allowFuel.clear(ClearReason::LambdaProtection);
	}

	if (noFiringUntilVvtSync()
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
		allowFuel.clear(m_rpmLimitReason);
	}

	// Limit fuel only on boost pressure (limiting spark bends valves)
	float mapCut = engineConfiguration->boostCutPressure;
	if (mapCut != 0) {
		// require drop of 'boostCutPressureHyst' kPa to resume fuel
		if (m_boostCutHysteresis.checkIfLimitIsExceeded(Sensor::getOrZero(SensorType::Map), mapCut, engineConfiguration->boostCutPressureHyst)) {
			allowFuel.clear(ClearReason::BoostCut);
		}
	}

	if (engine->rpmCalculator.isRunning()) {
		bool hasOilpSensor = Sensor::hasSensor(SensorType::OilPressure);
		auto oilp = Sensor::get(SensorType::OilPressure);
		uint16_t minOilPressure = engineConfiguration->minOilPressureAfterStart;

		// Only check if the setting is enabled and you have an oil pressure sensor
		if (minOilPressure > 0 && hasOilpSensor) {
			// Has it been long enough we should have pressure?
			bool isTimedOut = engine->rpmCalculator.getSecondsSinceEngineStart(nowNt) > 5.0f;

			// Only check before timed out
			if (!isTimedOut) {
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

		if (oilp && engineConfiguration->enableOilPressureProtect) {
			float minPressure = interpolate2d(rpm, config->minimumOilPressureBins, config->minimumOilPressureValues);
			bool isPressureSufficient = oilp.Value > minPressure;

			if (isPressureSufficient) {
				m_lowOilPressureTimer.reset(nowNt);
			}

			if (m_lowOilPressureTimer.hasElapsedSec(engineConfiguration->minimumOilPressureTimeout)) {
				allowFuel.clear(ClearReason::OilPressure);
			}
		}

		// check the maximum oil pressure
		float maxOilPressure = interpolate2d(rpm, config->maximumOilPressureBins, config->maximumOilPressureValues);
		if (maxOilPressure > 0 && hasOilpSensor) {
			if (oilp.Value < maxOilPressure) {
				m_highOilPressureTimer.reset(nowNt);
			}
			if (m_highOilPressureTimer.hasElapsedSec(engineConfiguration->maxOilPressureTimeout)) {
				allowFuel.clear(ClearReason::OilPressure);
			}
		}

	} else {
		// reset state in case of stalled engine
		m_hadOilPressureAfterStart = false;
		m_lowOilPressureTimer.reset(nowNt);
	}

	// If we're in engine stop mode, inhibit fuel
	if (shutdownController.isEngineStop(nowNt)) {
		/**
		 * todo: we need explicit clarification on why do we cut fuel but do not cut spark here!
		 */
		allowFuel.clear(ClearReason::StopRequested);
	}

	{
		// todo: we need to add some tests of this?
		// If duty cycle is high, impose a fuel cut rev limiter.
		// This is safer than attempting to limp along with injectors or a pump that are out of flow.
		// Two conditions will trigger a cut:
		// - An instantaneous excursion above maxInjectorDutyInstant
		// - A sustained excursion above maxInjectorDutySustained for a duration of >= maxInjectorDutySustainedTimeout
		// Only reset once below 20% duty to force the driver to lift off the pedal

		auto injDutyCycle = getInjectorDutyCycle(rpm);
		bool isOverInstantDutyCycle = injDutyCycle > engineConfiguration->maxInjectorDutyInstant;
		bool isOverSustainedDutyCycle = injDutyCycle > engineConfiguration->maxInjectorDutySustained;
		bool isUnderLowDuty = injDutyCycle < 20;

		if (!isOverSustainedDutyCycle) {
			// Duty cycle is OK, reset timer.
			m_injectorDutySustainedTimer.reset(nowNt);
		}

		// True if isOverSustainedDutyCycle has been true for longer than the timeout
		bool sustainedLimitTimedOut = m_injectorDutySustainedTimer.hasElapsedSec(engineConfiguration->maxInjectorDutySustainedTimeout);

		bool someLimitTripped = isOverInstantDutyCycle || sustainedLimitTimedOut;

		if (m_injectorDutyCutHysteresis.test(someLimitTripped, isUnderLowDuty)) {
			allowFuel.clear(ClearReason::InjectorDutyCycle);
			warning(ObdCode::CUSTOM_TOO_LONG_FUEL_INJECTION, "Injector duty cycle cut %.1f", injDutyCycle);
		}
	}

	{
		// GDI Fuel cut
		bool isGDIDriverInjectorTimeTooLong = engine->engineState.injectionDuration > engineConfiguration->mc33_t_hold_tot;

		if (isGdiEngine() && isGDIDriverInjectorTimeTooLong) {
			allowFuel.clear(ClearReason::GdiLimits);
			warning(ObdCode::CUSTOM_TOO_LONG_FUEL_INJECTION, "Injection duration excess PT2001 limits time: %.4f", engine->engineState.injectionDuration);
		}
	}

	// If the pedal is pushed while not running, cut fuel to clear a flood condition.
	if (!engine->rpmCalculator.isRunning() &&
		engineConfiguration->isCylinderCleanupEnabled &&
		Sensor::getOrZero(SensorType::DriverThrottleIntent) > CLEANUP_MODE_TPS) {
		allowFuel.clear(ClearReason::FloodClear);
	}
#endif // EFI_SHAFT_POSITION_INPUT

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

void LimpManager::reportEtbJammed() {
	m_allowEtb.clear(ClearReason::EtbProblem);
	setFaultRevLimit(/*rpm*/1500, ClearReason::EtbJammedRevLimit);
}

void LimpManager::fatalError() {
	m_allowEtb.clear(ClearReason::Fatal);
	m_allowIgnition.clear(ClearReason::Fatal);
	m_allowInjection.clear(ClearReason::Fatal);
	m_allowTriggerInput.clear(ClearReason::Fatal);

	setFaultRevLimit(/*rpm*/0, ClearReason::FatalErrorRevLimit);
}

void LimpManager::setFaultRevLimit(int limit, ClearReason rpmLimitReason) {
	// Only allow decreasing the limit
	// aka uses the limit of the worst fault to yet occur
	if (limit < m_faultRevLimit) {
	  m_faultRevLimit = limit;
	  m_rpmLimitReason = rpmLimitReason;
	}
}

#if EFI_ELECTRONIC_THROTTLE_BODY
bool LimpManager::allowElectronicThrottle() const {
	return m_allowEtb || engine->etbIgnoreJamProtection;
}
#endif // EFI_ELECTRONIC_THROTTLE_BODY

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
