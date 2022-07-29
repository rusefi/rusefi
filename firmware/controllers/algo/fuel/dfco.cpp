#include "pch.h"

bool DfcoController::getState() const {
	if (!engineConfiguration->coastingFuelCutEnabled) {
		return false;
	}

	const auto tps = Sensor::get(SensorType::DriverThrottleIntent);
	const auto clt = Sensor::get(SensorType::Clt);
	const auto map = Sensor::get(SensorType::Map);

	// If some sensor is broken, inhibit DFCO
	if (!tps || !clt || !map) {
		return false;
	}

	float rpm = Sensor::getOrZero(SensorType::Rpm);
	float vss = Sensor::getOrZero(SensorType::VehicleSpeed);

	bool mapActivate = map.Value < engineConfiguration->coastingFuelCutMap;
	bool tpsActivate = tps.Value < engineConfiguration->coastingFuelCutTps;
	bool cltActivate = clt.Value > engineConfiguration->coastingFuelCutClt;
	// True if throttle, MAP, and CLT are all acceptable for DFCO to occur
	bool dfcoAllowed = mapActivate && tpsActivate && cltActivate;

	bool rpmActivate = (rpm > engineConfiguration->coastingFuelCutRpmHigh);
	bool rpmDeactivate = (rpm < engineConfiguration->coastingFuelCutRpmLow);

	// greater than or equal so that it works if both config params are set to 0
	bool vssActivate = (vss >= engineConfiguration->coastingFuelCutVssHigh);
	bool vssDeactivate = (vss < engineConfiguration->coastingFuelCutVssLow);

	// RPM is high enough, VSS high enough, and DFCO allowed
	if (dfcoAllowed && rpmActivate && vssActivate) {
		return true;
	}

	// RPM too low, VSS too low, or DFCO not allowed
	if (!dfcoAllowed || rpmDeactivate || vssDeactivate) {
		return false;
	}

	// No conditions hit, no change to state (provides hysteresis)
	return m_isDfco;
}

void DfcoController::update() {
	// Run state machine
	bool newState = getState();

	// If fuel is cut, reset the timer
	if (newState) {
		m_timeSinceCut.reset();
	}

	m_isDfco = newState;
}

bool DfcoController::cutFuel() const {
	return m_isDfco;
}

float DfcoController::getTimeSinceCut() const {
	return m_timeSinceCut.getElapsedSeconds();
}
