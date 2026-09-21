#pragma once

#include <cstdint>
#include "../reset_cause.h"

// AT32F435/437 CRM_CTRLSTS reset flags. Bit 25 is reserved, not STM32 BORRSTF.
inline Reset_Cause_t decodeAt32ResetCause(uint32_t flags) {
	if (flags & (1U << 27)) {
		return Reset_Cause_POR;
	} else if (flags & (1U << 28)) {
		return Reset_Cause_Soft_Reset;
	} else if (flags & (1U << 29)) {
		return Reset_Cause_IWatchdog;
	} else if (flags & (1U << 30)) {
		return Reset_Cause_WWatchdog;
	} else if (flags & (1U << 31)) {
		return Reset_Cause_Illegal_Mode;
	} else if (flags & (1U << 26)) {
		// Internal reset sources can also assert the NRST flag.
		return Reset_Cause_NRST_Pin;
	}
	return Reset_Cause_Unknown;
}
