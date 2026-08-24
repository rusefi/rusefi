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
};
