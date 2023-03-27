#pragma once
#include "rusefi_types.h"
struct injector_model_s {
	// injectorLag(VBatt)
	// this value depends on a slow-changing VBatt value, so
	// we update it once in a while
	// offset 0
	float m_deadtime = (float)0;

	// fuel: injector pressureDelta
	// kPa
	// offset 4
	float pressureDelta = (float)0;

	// fuel: injector pressureRatio
	// offset 8
	float pressureRatio = (float)0;

};
static_assert(sizeof(injector_model_s) == 12);

