#include "pch.h"

#include "trigger_scope.h"

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
	// we're done with the buffer - let somebody else have it
	buffer = {};

	isRunning = false;
	engine->outputChannels.triggerScopeReady = false;
}

static int theta = 0;

// Retrieve the trace buffer
const BigBufferHandle& triggerScopeGetBuffer() {
	if (buffer) {
		for (size_t i = 0; i < buffer.size(); i++)
		{
			buffer.get<uint8_t>()[i] = 128 + 100 * sin((float)theta / 50);
			theta++;
		}
	}

	// engine->outputChannels.triggerScopeReady = false;

	return buffer;
}
