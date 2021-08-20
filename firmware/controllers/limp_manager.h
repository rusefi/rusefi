#pragma once

#include "rusefi_types.h"

#include <cstdint>

enum class ClearReason : uint8_t {
	None, // 0
	Fatal,
	Settings,
	HardLimit, // 3
	FaultRevLimit,
	BoostCut, // 5
	OilPressure,
	StopRequested, // 7
	EtbProblem,
};

// Only allows clearing the value, but never resetting it.
class Clearable {
public:
	Clearable() : m_value(true) {}
	Clearable(bool value) : m_value(value) {
		if (!m_value) {
			clearReason = ClearReason::Settings;
		}
	}

	void clear(ClearReason clearReason) {
		m_value = false;
		this->clearReason = clearReason;
	}

	operator bool() const {
		return m_value;
	}

	ClearReason clearReason = ClearReason::None;
private:
	bool m_value = true;
};

struct LimpState {
	const bool value;
	const ClearReason reason;

	// Implicit conversion operator to bool, so you can do things like if (myResult) { ... }
	constexpr explicit operator bool() const {
		return value;
	}
};

class LimpManager {
public:
	// This is called from periodicFastCallback to update internal state
	void updateState(int rpm, efitick_t nowNt);

	// Other subsystems call these APIs to determine their behavior
	bool allowElectronicThrottle() const;

	LimpState allowInjection() const;
	LimpState allowIgnition() const;

	bool allowTriggerInput() const;

	// Other subsystems call these APIs to indicate a problem has occured
	void etbProblem();
	void fatalError();
	void stopEngine();

	bool isEngineStop(efitick_t nowNt) const;
	float getTimeSinceEngineStop(efitick_t nowNt) const;

private:
	void setFaultRevLimit(int limit);

	// Start with no fault rev limit
	int32_t m_faultRevLimit = INT32_MAX;

	Clearable m_allowEtb;
	Clearable m_allowInjection;
	Clearable m_allowIgnition;
	Clearable m_allowTriggerInput;

	Clearable m_transientAllowInjection = true;
	Clearable m_transientAllowIgnition = true;

	bool m_hadOilPressureAfterStart = false;

	Timer m_engineStopTimer;
};
