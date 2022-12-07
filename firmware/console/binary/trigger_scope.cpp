#include "pch.h"

static BigBufferHandle buffer;

// Enable one buffer's worth of perf tracing, and retrieve the buffer size in bytes
void triggerScopeEnable() {
	buffer = getBigBuffer(BigBufferUser::TriggerScope);

	// TODO: trigger ADC

	engine->outputChannels.triggerScopeReady = true;
}

// Retrieve the trace buffer
const BigBufferHandle triggerScopeGetBuffer() {
	return std::move(buffer);
}
