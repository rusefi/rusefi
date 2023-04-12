/**
 * @file	state_sequence.cpp
 *
 * @date May 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "state_sequence.h"
#include "trigger_structure.h"

void MultiChannelStateSequence::checkSwitchTimes(const float scale) const {
	efiAssertVoid(ObdCode::CUSTOM_ERR_WAVE_1, phaseCount > 0, "StateSequence cannot be empty");
	if (getSwitchTime(phaseCount - 1) != 1) {
#if EFI_UNIT_TEST
		for (int index = 0;index < phaseCount;index ++) {
			printf("switch time index=%d angle=%f\n", index, getSwitchTime(index));
		}
#endif // EFI_UNIT_TEST

		firmwareError(ObdCode::CUSTOM_ERR_WAVE_1, "[count=%d] last switch time has to be 1/%f not %.2f/%f",
				phaseCount,
			      scale, getSwitchTime(phaseCount - 1),
			      scale * getSwitchTime(phaseCount - 1));
		return;
	}
	for (int i = 0; i < phaseCount - 1; i++) {
		if (getSwitchTime(i) >= getSwitchTime(i + 1)) {
			firmwareError(ObdCode::CUSTOM_ERR_WAVE_2, "invalid switchTimes @%d: %.2f/%.2f",
				      i, getSwitchTime(i), getSwitchTime(i + 1));
		}
	}
}

int MultiChannelStateSequence::findInsertionAngle(const float angle) const {
	for (int i = phaseCount - 1; i >= 0; i--) {
		if (angle > getSwitchTime(i))
			return i + 1;
	}
	return 0;
}

expected<int> MultiChannelStateSequence::findAngleMatch(const float angle) const {
	for (int i = 0; i < phaseCount; i++) {
		if (isSameF(getSwitchTime(i), angle))
			return i;
	}
	return unexpected;
}
