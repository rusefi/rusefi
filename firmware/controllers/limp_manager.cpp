#include "limp_manager.h"
#include "engine.h"
#include "efilib.h"

EXTERN_ENGINE;

void LimpManager::updateState(int rpm) {
	Clearable allowFuel = CONFIG(isInjectionEnabled);
	Clearable allowSpark = CONFIG(isIgnitionEnabled);

	// User-configured hard RPM limit
	if (rpm > engine->getRpmHardLimit(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		if (CONFIG(cutFuelOnHardLimit)) {
			allowFuel.clear();
		}

		if (CONFIG(cutSparkOnHardLimit)) {
			allowSpark.clear();
		}
	}

	// Force fuel limiting on the fault rev limit
	if (rpm > m_faultRevLimit) {
		allowFuel.clear();
	}

	// Limit fuel only on boost pressure (limiting spark bends valves)
	if (CONFIG(boostCutPressure) != 0) {
		if (Sensor::get(SensorType::Map).value_or(0) > CONFIG(boostCutPressure)) {
			allowFuel.clear();
		}
	}

	{
		uint16_t minCrankingOilPressure = CONFIG(minCrankingOilPressure);

		// Only check if the setting is enabled and we're cranking
		if (minCrankingOilPressure > 0 && ENGINE(rpmCalculator).isCranking()) {
			auto oilp = Sensor::get(SensorType::OilPressure);

			// If the sensor is working and pressure is too low, cut fuel
			if (oilp && oilp.Value < minCrankingOilPressure) {
				allowFuel.clear();
			}
		}
	}

	m_transientAllowInjection = allowFuel;
	m_transientAllowIgnition = allowSpark;
}

void LimpManager::etbProblem() {
	m_allowEtb.clear();
	setFaultRevLimit(1500);
}

void LimpManager::fatalError() {
	m_allowEtb.clear();
	m_allowIgnition.clear();
	m_allowInjection.clear();
	m_allowTriggerInput.clear();

	setFaultRevLimit(0);
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

bool LimpManager::allowInjection() const {
	return m_transientAllowInjection && m_allowInjection;
}

bool LimpManager::allowIgnition() const {
	return m_transientAllowIgnition && m_allowIgnition;
}
