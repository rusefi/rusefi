#pragma once

#include "shutdown_controller.h"
#include "max_limit_with_hysteresis.h"

#include <cstdint>

// Keep this list in sync with fuelIgnCutCodeList in tunerstudio.template.ini!
enum class ClearReason : uint8_t {
	None, // 0
	Fatal, // 1
	Settings, // 2
	HardLimit, // 3
	EtbJammedRevLimit, // 4 means 1500 RPM limit in ETB jam was detected
	BoostCut, // 5
	OilPressure, // 6
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
	GdiComms, // 18
	PleaseBrake, // 19
	FatalErrorRevLimit, // 20
	GdiLimits, // 21
	GdiPumpLimit, // 22

	// Keep this list in sync with fuelIgnCutCodeList in tunerstudio.template.ini!
	// todo: add a code generator between ClearReason and fuelIgnCutCodeList in tunerstudio.template.ini
};

enum class EtbStatus : uint8_t {
	None, // 0
	EngineStopped, // 1
	TpsError, // 2
	PpsError, // 3
	IntermittentTps, // 4
	AutoTune, // 5
	Lua, // 6
	AutoCalibrate, // 7
	NotConfigured, // 8
	Redundancy, // 9
	IntermittentPps, // 10
	JamDetected, // 11
	// keep this list in sync with etbCutCodeList in tunerstudio.template.ini!
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

	void clear(ClearReason p_clearReason) {
		if (m_value) {
			m_value = false;
			clearReason = p_clearReason;
		}
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

class LimpManager : public EngineModule {
public:
	ShutdownController shutdownController;
	// Mockable<> interface
	using interface_t = LimpManager;

	// This is called from periodicFastCallback to update internal state
	void updateState(float rpm, efitick_t nowNt);

	void onFastCallback() override;
	void onIgnitionStateChanged(bool ignitionOn) override;

	// Other subsystems call these APIs to determine their behavior
	bool allowElectronicThrottle() const;

	LimpState allowInjection() const;
	LimpState allowIgnition() const;

	float getTimeSinceAnyCut() const;

	bool allowTriggerInput() const;

	void updateRevLimit(float rpm);
	angle_t getLimitingTimingRetard() const;
	float getLimitingFuelCorrection() const;

	// Other subsystems call these APIs to indicate a problem has occurred
	void reportEtbJammed();
	void fatalError();
	Timer externalGdiCanBusComms;

private:
	void setFaultRevLimit(int limit, ClearReason rpmLimitReason);

	Hysteresis m_revLimitHysteresis;
	MaxLimitWithHysteresis<> m_boostCutHysteresis;
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

	angle_t m_timingRetard = 0;
	float m_fuelCorrection = 1.0f;

	// todo: migrate to engineState->desiredRpmLimit to get this variable logged
	float m_revLimit;
	ClearReason m_rpmLimitReason = ClearReason::None;
	float resumeRpm;

	// Tracks how long since a cut (ignition or fuel) was active for any reason
	Timer m_lastCutTime;

	// Tracks how long injector duty has been over the sustained limit
	Timer m_injectorDutySustainedTimer;

	// Tracks how long oil pressure has been out of thresholds
	Timer m_lowOilPressureTimer;
	Timer m_highOilPressureTimer;
};

#if EFI_ENGINE_CONTROL
LimpManager * getLimpManager();
#endif // EFI_ENGINE_CONTROL


