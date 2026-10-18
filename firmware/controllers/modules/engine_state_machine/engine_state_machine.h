#pragma once

#include "engine_module.h"
#include "engine_state_machine_state_generated.h"
#include <rusefi/timer.h>

enum class EngineStateMachineState : uint8_t {
	Off = 0,
	Cranking = 1,
	Afterstart = 2,
	Idle = 3,
	Coasting = 4,
	Transient = 5,
	WOT = 6,
	Cruising = 7,
	Overrun = 8,
	LaunchControl = 9,
	Upshifting = 10,
	Downshifting = 11,
	// 12 and 13 are reserved for separate AlphaX operating-mode features.
	Accelerating = 14,
	Decelerating = 15,
};

static constexpr efitimems_t SM_SHIFT_TIMEOUT_MS = 3000;

class EngineStateMachine : public engine_state_machine_state_s, public EngineModule {
public:
	using interface_t = EngineStateMachine;
	void setDefaultConfiguration() override;
	void onSlowCallback() override;
	void onEngineStop() override;
	void onConfigurationChange(engine_configuration_s const* previousConfig) override;
	EngineStateMachineState getCurrentState() const;
	bool isEnabled() const;
	bool isTransmissionEngaged() const;
	float getLastRpmRate() const { return m_lastRpmRate; }

private:
	void reset();
	EngineStateMachineState determineState(float rpm, float tps, float vss, bool vssValid);
	void updateShiftDetection(float vss, bool vssValid, efitimems_t nowMs);
	void updateShiftAccumulator(float rpm, float vss, bool vssValid, efitimems_t nowMs);
	bool evaluateShiftDirection(bool isUpshift, float currentVss);
	float recordRpmSampleAndComputeRate(float rpm, efitimems_t nowMs);
	void updateTempOverlay();

	EngineStateMachineState m_currentState = EngineStateMachineState::Off;
	Timer m_transientHoldTimer;
	Timer m_accelHoldTimer;
	Timer m_accelStateTimer;
	Timer m_decelStateTimer;
	bool m_shiftWindowOpen = false;
	bool m_shiftIsUpshift = false;
	efitimems_t m_shiftWindowOpenMs = 0;
	bool m_prevClutchUp = false;
	bool m_prevClutchDown = false;
	bool m_clutchHasSample = false;
	bool m_shiftLatched = false;
	bool m_shiftLatchedIsUpshift = false;
	efitimems_t m_shiftLatchUntilMs = 0;
	float m_shiftAccumulator = 0;
	efitimems_t m_accumulatorLastMs = 0;
	float m_accumulatorLastRpm = 0;
	float m_accumulatorLastVss = 0;
	bool m_accumulatorHasSample = false;
	// Held between cadence updates; state classification reads the previous callback's rate.
	float m_lastRpmRate = 0;
	efitimems_t m_rpmRateAnchorMs = 0;
	float m_rpmRateAnchorRpm = 0;
	bool m_rpmRateHasAnchor = false;
};
