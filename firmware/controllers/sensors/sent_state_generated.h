#pragma once
#include "rusefi_types.h"
struct sent_state_s {
	// ETB: SENT value0
	// value
	// offset 0
	uint16_t value0 = (uint16_t)0;
	// ETB: SENT value1
	// value
	// offset 2
	uint16_t value1 = (uint16_t)0;
	// ETB: SENT error rate
	// ratio
	// offset 4
	float errorRate = (float)0;
};
static_assert(sizeof(sent_state_s) == 8);
static_assert(offsetof(sent_state_s, value0) == 0);
static_assert(offsetof(sent_state_s, value1) == 2);
static_assert(offsetof(sent_state_s, errorRate) == 4);

