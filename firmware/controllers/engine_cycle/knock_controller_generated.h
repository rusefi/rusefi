#pragma once
#include "rusefi_types.h"
struct knock_controller_s {
	// Knock: Current level
	// Volts
	// offset 0
	float m_knockLevel = (float)0;
	// Knock: Cyl
	// dBv
	// offset 4
	int8_t m_knockCyl[12];
	// Knock: Retard
	// deg
	// offset 16
	angle_t m_knockRetard = (angle_t)0;
	// Knock: Threshold
	// offset 20
	float m_knockThreshold = (float)0;
	// Knock: Count
	// offset 24
	uint32_t m_knockCount = (uint32_t)0;
	// Knock: Max retard
	// offset 28
	float m_maximumRetard = (float)0;
};
static_assert(sizeof(knock_controller_s) == 32);
static_assert(offsetof(knock_controller_s, m_knockLevel) == 0);
static_assert(offsetof(knock_controller_s, m_knockCyl) == 4);
static_assert(offsetof(knock_controller_s, m_knockRetard) == 16);
static_assert(offsetof(knock_controller_s, m_knockThreshold) == 20);
static_assert(offsetof(knock_controller_s, m_knockCount) == 24);
static_assert(offsetof(knock_controller_s, m_maximumRetard) == 28);

