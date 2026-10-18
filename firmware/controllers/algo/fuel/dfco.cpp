// Deceleration Fuel Cut-off

#include "pch.h"

#include "dfco.h"
#include "closed_loop_fuel.h"
#include "engine_state_machine.h"

bool DfcoController::commonGuards() const {
	if (!engineConfiguration->coastingFuelCutEnabled) {
		return false;
	}

	if (checkIfTuningVeNow()) {
	  return false;
	}

	if (engine->engineState.lua.disableDecelerationFuelCutOff) {
		return false;
	}

	const auto clt = Sensor::get(SensorType::Clt);
	if (!clt || clt.Value <= engineConfiguration->coastingFuelCutClt) {
		return false;
	}

	// MAP sensor is optional, only inhibit if the sensor is present but broken
	if (Sensor::hasSensor(SensorType::Map)) {
		const auto map = Sensor::get(SensorType::Map);
		if (!map || m_mapHysteresis.test(map.Value,
			engineConfiguration->coastingFuelCutMap + 1, engineConfiguration->coastingFuelCutMap - 1)) {
			return false;
		}
	}

	return true;
}

bool DfcoController::overrunActive() const {
	const auto tps = Sensor::get(SensorType::DriverThrottleIntent);
	if (!tps) {
		return false;
	}
	if (engineConfiguration->coastingFuelCutRequiresGear && Sensor::getOrZero(SensorType::DetectedGear) <= 0) {
		return false;
	}

	float rpm = Sensor::getOrZero(SensorType::Rpm);
	float vss = Sensor::getOrZero(SensorType::VehicleSpeed);

	bool tpsActivate = tps.Value < engineConfiguration->coastingFuelCutTps;

	bool rpmActivate = (rpm > engineConfiguration->coastingFuelCutRpmHigh);
	bool rpmDeactivate = (rpm < engineConfiguration->coastingFuelCutRpmLow);

	// greater than or equal so that it works if both config params are set to 0
	bool vssActivate = (vss >= engineConfiguration->coastingFuelCutVssHigh);
	bool vssDeactivate = (vss < engineConfiguration->coastingFuelCutVssLow);

	if (tpsActivate && rpmActivate && vssActivate) {
		return true;
	}

	if (!tpsActivate || rpmDeactivate || vssDeactivate) {
		return false;
	}

	// No conditions hit, no change to state (provides hysteresis)
	return m_isDfco;
}

bool DfcoController::getState(const EngineStateMachine& sm) const {
	if (!commonGuards()) {
		return false;
	}

	auto mode = engineConfiguration->dfcoFuelCutMode;
	bool wantsOverrun = mode == dfco_fuel_cut_mode_e::Overrun || mode == dfco_fuel_cut_mode_e::Both;
	bool wantsDecel = mode == dfco_fuel_cut_mode_e::Decel || mode == dfco_fuel_cut_mode_e::Both;
	if (wantsOverrun && overrunActive()) {
		return true;
	}

	// [tag:state_aware_dfco] Decel uses the SM classification, independently of the
	// overrun TPS/RPM/VSS/gear gates. Only an absent VSS permits the RPM-rate fallback;
	// a configured but failed sensor must not silently change detection strategy.
	if (wantsDecel && sm.isEnabled()) {
		if (sm.engineSmIsDecelerating) {
			return true;
		}
		if (!Sensor::hasSensor(SensorType::VehicleSpeed)) {
			float decelThreshold = engineConfiguration->smDecelRateThreshold;
			return decelThreshold > 0 && sm.getLastRpmRate() < -decelThreshold;
		}
	}

	return false;
}

void DfcoController::update() {
	auto& sm = engine->module<EngineStateMachine>().unmock();
	bool newState = getState(sm);
	// Shift assistance needs fuel. A suppressed cut also restarts the entry delay.
	if (sm.isEnabled() && (sm.engineSmIsUpshifting || sm.engineSmIsDownshifting)) {
		newState = false;
	}

	// If fuel is cut, reset the timer
	if (newState) {
		m_timeSinceCut.reset();
	} else {
		// If fuel is not cut, reset the not-cut timer
		m_timeSinceNoCut.reset();
	}

	m_isDfco = newState;
}

bool DfcoController::cutFuel() const {
	float cutDelay = engineConfiguration->dfcoDelay;

	// 0 delay means cut immediately, aka timer has always expired
	bool hasBeenDelay = (cutDelay == 0) || m_timeSinceNoCut.hasElapsedSec(cutDelay);

	return m_isDfco && hasBeenDelay;
}

float DfcoController::getTimeSinceCut() const {
	return m_timeSinceCut.getElapsedSeconds();
}

float DfcoController::getTimingRetard() const {
	float cutTiming = clampF(0, engineConfiguration->dfcoRetardDeg, 30);

	if (m_isDfco) {
		// While cut, always retard timing
		return cutTiming;
	} else {
		float timeSinceCut = m_timeSinceCut.getElapsedSeconds();
		float rampInTime = engineConfiguration->dfcoRetardRampInTime;

		if (timeSinceCut > rampInTime) {
			// Normal operation, no retard
			return 0;
		} else {
			return interpolateClamped(0, cutTiming, 0.5, 0, timeSinceCut);
		}
	}
}
