#pragma once

#include "engine_ptr.h"
#include "rusefi_types.h"

#include <cstdint>

// Only allows clearing the value, but never resetting it.
class Clearable {
public:
	Clearable() : m_value(true) {}
	Clearable(bool value) : m_value(value) {}

	void clear() {
		m_value = false;
	}

	operator bool() const {
		return m_value;
	}

private:
	bool m_value = true;
};

class LimpManager {
public:
	DECLARE_ENGINE_PTR;

	// This is called from periodicFastCallback to update internal state
	void updateState(int rpm, efitick_t nowNt);

	// Other subsystems call these APIs to determine their behavior
	bool allowElectronicThrottle() const;

	bool allowInjection() const;
	bool allowIgnition() const;

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

	bool m_transientAllowInjection = true;
	bool m_transientAllowIgnition = true;

	bool m_hadOilPressureAfterStart = false;

	Timer m_engineStopTimer;
};
