#pragma once

#include "shutdown_controller.h"

#include <cstdint>

// Keep this list in sync with fuelIgnCutCodeList in rusefi.input!
enum class ClearReason : uint8_t {
	None, // 0
	Fatal,
	Settings,
	HardLimit, // 3
	FaultRevLimit,
	BoostCut, // 5
	OilPressure,
	StopRequested, // 7
	EtbProblem, // 8
	LaunchCut, // 9
	InjectorDutyCycle, // 10
	FloodClear, // 11
	EnginePhase, // 12
	KickStart, // 13
	IgnitionOff, // 14
	Lua, // 15
	ACR, // 16 - Harley Automatic Compression Release
	LambdaProtection, // 17

	// Keep this list in sync with fuelIgnCutCodeList in rusefi.input!
	// todo: add a code generator between ClearReason and fuelIgnCutCodeList in rusefi.input
};

enum class TpsState : uint8_t {
	None, // 0
	EngineStopped,
	TpsError,
	PpsError, // 3
	IntermittentTps,
	PidJitter,
	Lua, // 6
	Manual,
	NotConfigured,
	Redundancy, // 9
	IntermittentPps,
	// keep this list in sync with etbCutCodeList in rusefi.input!
};

// Only allows clearing the value, but never resetting it.
class Clearable {
public:
	Clearable() : m_value(true) {}
	Clearable(bool value) : m_value(value) {
		if (!m_value) {
			m_clearReason = ClearReason::Settings;
		}
	}

	void clear(ClearReason clearReason) {
		if (m_value) {
			m_value = false;
			m_clearReason = clearReason;
		}
	}

	operator bool() const {
		return m_value;
	}

	ClearReason why() const {
		return m_clearReason;
	}

private:
	bool m_value = true;
	ClearReason m_clearReason = ClearReason::None;
};

struct LimpState {
	const bool value;
	const ClearReason reason;

	// Implicit conversion operator to bool, so you can do things like if (myResult) { ... }
	constexpr explicit operator bool() const {
		return value;
	}
};

class Hysteresis {
public:
	// returns true if value > rising, false if value < falling, previous if falling < value < rising.
	bool test(float value, float rising, float falling) {
		return test(value > rising, value < falling);
	}

	bool test (bool risingCondition, bool fallingCondition) {
		if (risingCondition) {
			m_state = true;
		} else if (fallingCondition) {
			m_state = false;
		}

		return m_state;
	}

private:
	bool m_state = false;
};

class LimpManager : public EngineModule {
public:
	ShutdownController shutdownController;

	// This is called from periodicFastCallback to update internal state
	void updateState(int rpm, efitick_t nowNt);

	void onFastCallback() override;
	void onIgnitionStateChanged(bool ignitionOn) override;

	// Other subsystems call these APIs to determine their behavior
	bool allowElectronicThrottle() const;

	LimpState allowInjection() const;
	LimpState allowIgnition() const;

	float getTimeSinceAnyCut() const;

	bool allowTriggerInput() const;

	// Other subsystems call these APIs to indicate a problem has occurred
	void reportEtbProblem();
	void fatalError();

private:
	void setFaultRevLimit(int limit);

	Hysteresis m_revLimitHysteresis;
	Hysteresis m_boostCutHysteresis;
	Hysteresis m_injectorDutyCutHysteresis;

	// Start with no fault rev limit
	int32_t m_faultRevLimit = INT32_MAX;

	Clearable m_allowEtb;
	Clearable m_allowInjection;
	Clearable m_allowIgnition;
	Clearable m_allowTriggerInput;

	Clearable m_transientAllowInjection = true;
	Clearable m_transientAllowIgnition = true;

	bool m_hadOilPressureAfterStart = false;

	// Ignition switch state
	bool m_ignitionOn = false;

	// Tracks how long since a cut (ignition or fuel) was active for any reason
	Timer m_lastCutTime;

	// Tracks how long injector duty has been over the sustained limit
	Timer m_injectorDutySustainedTimer;
};

LimpManager * getLimpManager();


