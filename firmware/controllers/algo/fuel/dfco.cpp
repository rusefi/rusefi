#include "pch.h"

bool DfcoController::getState() const {
	if (!engineConfiguration->coastingFuelCutEnabled) {
		return false;
	}

	auto rpm = Sensor::getOrZero(SensorType::Rpm);
	const auto [tpsValid, tpsPos] = Sensor::get(SensorType::Tps1);
	const auto [cltValid, clt] = Sensor::get(SensorType::Clt);
	const auto [mapValid, map] = Sensor::get(SensorType::Map);

	// If some sensor is broken, inhibit DFCO
	if (!tpsValid || !cltValid || !mapValid) {
		return false;
	}

	bool mapActivate = map < engineConfiguration->coastingFuelCutMap;
	bool tpsActivate = tpsPos < engineConfiguration->coastingFuelCutTps;
	bool cltActivate = clt > engineConfiguration->coastingFuelCutClt;
	// True if throttle, MAP, and CLT are all acceptable for DFCO to occur
	bool dfcoAllowed = mapActivate && tpsActivate && cltActivate;

	bool rpmActivate = (rpm > engineConfiguration->coastingFuelCutRpmHigh);
	bool rpmDeactivate = (rpm < engineConfiguration->coastingFuelCutRpmLow);

	// RPM is high enough, and DFCO allowed
	if (dfcoAllowed && rpmActivate) {
		return true;
	}

	// RPM too low, or DFCO not allowed
	if (!dfcoAllowed || rpmDeactivate) {
		return false;
	}

	// No conditions hit, no change to state (provides hysteresis)
	return m_isDfco;
}

void DfcoController::update() {
	// Run state machine
	m_isDfco = getState();
}

bool DfcoController::cutFuel() const {
	return m_isDfco;
}
