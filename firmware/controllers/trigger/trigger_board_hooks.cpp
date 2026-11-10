/**
 * @file trigger_board_hooks.cpp
 *
 * Weak defaults for trigger board hooks.
 *
 * IMPORTANT: these definitions must NOT live in the same translation unit as
 * their call sites (trigger_central.cpp calls boardTriggerCallback,
 * trigger_decoder.cpp calls triggerGetToothProfileFactor). GCC LTO binds a
 * same-TU call to the weak body (dead-call elimination / const folding,
 * which noinline does not prevent) BEFORE the linker can select a board's
 * strong override - the board hook silently stops working. Keep them here.
 */

#include "pch.h"

#include "trigger_central.h"
#include "trigger_decoder.h"

PUBLIC_API_WEAK __attribute__((noinline)) void boardTriggerCallback(efitick_t timestamp, float currentPhase) {
	UNUSED(timestamp);
	UNUSED(currentPhase);
}

PUBLIC_API_WEAK __attribute__((noinline)) void boardRawTriggerEdge(int signalIndex, bool isRising, efitick_t timestamp) {
	UNUSED(signalIndex);
	UNUSED(isRising);
	UNUSED(timestamp);
}

PUBLIC_API_WEAK __attribute__((noinline)) float triggerGetToothProfileFactor(int) {
	return 1.0f;
}

PUBLIC_API_WEAK __attribute__((noinline)) void boardTriggerSyncEvent(char, int, float, float) {
}

PUBLIC_API_WEAK __attribute__((noinline)) void triggerObserveGapShift(float) {
	// weak default: no board VR amplitude model to train
}
