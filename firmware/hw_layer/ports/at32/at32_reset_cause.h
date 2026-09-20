#pragma once

#include <cstdint>
#include "../reset_cause.h"

inline Reset_Cause_t decodeAt32ResetCause(uint32_t /*flags*/) {
	return Reset_Cause_Unknown;
}
