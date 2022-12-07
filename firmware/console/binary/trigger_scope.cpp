#include "pch.h"

static BigBufferHandle buffer;

static bool isRunning = false;

// Enable one buffer's worth of perf tracing, and retrieve the buffer size in bytes
void triggerScopeEnable() {
	buffer = getBigBuffer(BigBufferUser::TriggerScope);

	// TODO: trigger ADC

	isRunning = true;
	engine->outputChannels.triggerScopeReady = true;
}

void triggerScopeDisable() {
	isRunning = false;
	engine->outputChannels.triggerScopeReady = false;
}

// Retrieve the trace buffer
const BigBufferHandle triggerScopeGetBuffer() {
	return std::move(buffer);
}
