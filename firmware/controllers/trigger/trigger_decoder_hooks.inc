// Shared fallback bodies. Include in the Windows caller or the ELF defaults TU.
PUBLIC_API_WEAK __attribute__((noinline)) float triggerGetToothProfileFactor(int) {
	return 1.0f;
}

PUBLIC_API_WEAK __attribute__((noinline)) void boardTriggerSyncEvent(char, int, float, float) {
}

PUBLIC_API_WEAK __attribute__((noinline)) void triggerObserveGapShift(float) {
	// weak default: no board VR amplitude model to train
}
