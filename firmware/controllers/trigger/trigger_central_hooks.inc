// Shared fallback bodies. Include in the Windows caller or the ELF defaults TU.
PUBLIC_API_WEAK __attribute__((noinline)) void boardTriggerCallback(efitick_t timestamp, float currentPhase) {
	UNUSED(timestamp);
	UNUSED(currentPhase);
}

PUBLIC_API_WEAK __attribute__((noinline)) void boardRawTriggerEdge(int signalIndex, bool isRising, efitick_t timestamp) {
	UNUSED(signalIndex);
	UNUSED(isRising);
	UNUSED(timestamp);
}
