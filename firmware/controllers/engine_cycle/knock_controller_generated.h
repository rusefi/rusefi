#pragma once
#include "rusefi_types.h"
struct knock_controller_s {
	// Knock Degrees retarded
	// larger number = more retard
	// offset 0
	angle_t m_knockRetard = (angle_t)0;
	// offset 4
	float m_knockThreshold = (float)0;
	// offset 8
	uint32_t m_knockCount = (uint32_t)0;
	// offset 12
	float m_maximumRetard = (float)0;
};
static_assert(sizeof(knock_controller_s) == 16);

