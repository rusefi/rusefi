#pragma once

// live data struct for this module
#include "injector_deadtime_autotune_state_generated.h"

/**
 * Injector deadtime autotune
 *
 * measure deadtime error via the sequential<->batch settled-STFT-trim differential
 */
class InjectorDeadtimeAutotune : public EngineModule, public injector_deadtime_autotune_state_s {
public:
	void onSlowCallback() override;

#if EFI_ENGINE_CONTROL && EFI_LUA && (defined(STM32F7) || EFI_UNIT_TEST)
	// Lua owns the estimator. These are temporary, independently leased controls.
	static constexpr float MaxDeadtimeAddMs = 2.0f;
	bool setInjectionMode(int mode); // -1 releases; otherwise sequential/batch only
	bool setDeadtimeAdd(float milliseconds);
	bool setLtftLearningDisabled(bool disabled);
	float getDeadtimeAdd() const;
	bool isLtftLearningDisabled() const;
	injection_mode_e getInjectionMode(injection_mode_e configured) const;
	bool isTransitioning() const;
	bool hasModeOverride() const;
	bool isSchedulingBlocked() const;

	// Bracket fuel calculation: drain queued pulses, change mode, calculate mass,
	// then rebuild the schedule before permitting new pulses.
	void beginFastCallback();
	void endFastCallback();
	void injectionScheduled(unsigned closes);
	void injectionClosed();
	void resetLua();
	void onLuaError();
	void onEngineStop() override;
	void onConfigurationChange(engine_configuration_s const*) override;

private:
	bool canTune() const;
	void clearRequests();
	int m_requestedMode = -1;
	int m_appliedMode = -1;
	float m_deadtimeAdd = 0;
	bool m_disableLtftLearning = false;
	bool m_luaError = false;
	bool m_blockScheduling = false;
	bool m_rebuild = false;
	unsigned m_pendingCloses = 0;
	efitick_t m_modeExpires = 0;
	efitick_t m_deadtimeExpires = 0;
	efitick_t m_ltftExpires = 0;
#endif
};
