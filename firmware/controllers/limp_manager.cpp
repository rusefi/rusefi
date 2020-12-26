#include "limp_manager.h"
#include "efilib.h"
#include "engine.h"
#include "map.h"

EXTERN_ENGINE;

void LimpManager::updateState(int rpm) {
	// User-configured hard RPM limit
	bool isRevLimited = rpm > engine->getRpmHardLimit(PASS_ENGINE_PARAMETER_SIGNATURE);

	// TODO: user configurable what gets limited
	bool limitFuel = isRevLimited;
	bool limitSpark = isRevLimited;

	// Force fuel limiting on the fault rev limit
	if (rpm > m_faultRevLimit) {
		limitFuel = true;
	}

	// Limit fuel only on boost pressure
	if (CONFIG(boostCutPressure) != 0) {
		if (getMap(PASS_ENGINE_PARAMETER_SIGNATURE) > CONFIG(boostCutPressure)) {
			limitFuel = true;
		}
	}

	m_transientLimitInjection = limitFuel;
	m_transientLimitIgnition = limitSpark;
}

void LimpManager::etbProblem() {
	m_allowEtb.clear();
	setFaultRevLimit(1500);
}

void LimpManager::fatalError() {
	m_allowEtb.clear();
	m_allowIgnition.clear();
	m_allowInjection.clear();

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
	return !m_transientLimitInjection && m_allowInjection;
}

bool LimpManager::allowIgnition() const {
	return !m_transientLimitIgnition && m_allowIgnition;
}
