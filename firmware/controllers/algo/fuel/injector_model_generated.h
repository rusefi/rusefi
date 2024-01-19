#pragma once
#include "rusefi_types.h"
struct injector_model_s {
	// Fuel: injector lag
	// ms
	// offset 0
	float m_deadtime = (float)0;
	// fuel: Injector pressure delta
	// kPa
	// offset 4
	float pressureDelta = (float)0;
	// fuel: Injector pressure ratio
	// offset 8
	float pressureRatio = (float)0;
};
static_assert(sizeof(injector_model_s) == 12);
static_assert(offsetof(injector_model_s, m_deadtime) == 0);
static_assert(offsetof(injector_model_s, pressureDelta) == 4);
static_assert(offsetof(injector_model_s, pressureRatio) == 8);

