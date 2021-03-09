/**
 * @file	loggingcentral.h
 *
 * @date Mar 8, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */
#pragma once

class Logging;

void startLoggingProcessor();

const char* swapOutputBuffers(int *actualOutputBufferSize);

namespace priv
{
	// internal implementation, use scheduleMsg below
	void scheduleMsgInternal(const char *fmt, ...);
}

// "normal" logging messages need a header and footer, so put them in
// the format string at compile time
#define scheduleMsg(logging, fmt, ...) priv::scheduleMsgInternal(PROTOCOL_MSG DELIMETER fmt DELIMETER, ##__VA_ARGS__)

/**
 * This is the legacy function to copy the contents of a local Logging object in to the output buffer
 */
void scheduleLogging(Logging *logging);
