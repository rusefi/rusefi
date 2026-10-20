/**
 * @file	loggingcentral.h
 *
 * @date Mar 8, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */
#pragma once

#include <cstddef>
#include "generated_lookup_meta.h"

#include "tunerstudio_io.h"

class Logging;

// Makes the internal line buffers available for logging, see loggingcentral.cpp
void startLoggingProcessor();

// Called from the binary TS thread: sends as many queued buffers as fit in one packet
// and releases each of them back to its owner
size_t loggingSendOutputData(TsChannelBase* tsChannel);

#if EFI_UNIT_TEST
// Discards all queued buffers and makes every line buffer free again
void resetLoggingForUnitTest();
#endif

namespace priv
{
	// Fixes up a possibly-truncated log line: guarantees the buffer stays
	// null-terminated and re-adds the trailing LOG_DELIMITER framing lost to
	// truncation. untruncatedLen is chvsnprintf's return value (the length the
	// full message would have had); returns the actual in-buffer string length.
	// See https://github.com/rusefi/rusefi/issues/10159
	size_t terminateLogLine(char* buffer, size_t bufferSize, size_t untruncatedLen);

	// internal implementation, use efiPrintf below
	void efiPrintfInternal(const char *fmt, ...)
		#if EFI_PROD_CODE
			__attribute__ ((format (printf, 1, 2)))
		#endif
			;
}

// "normal" logging messages need a header and footer, so put them in
// the format string at compile time.
// Limits: one efiPrintf call produces at most line buffer size - 1 characters
// INCLUDING the PROTOCOL_MSG + LOG_DELIMITER framing; anything longer is
// truncated, not split.
#define efiPrintfProto(proto, fmt, ...) priv::efiPrintfInternal(proto LOG_DELIMITER fmt LOG_DELIMITER, ##__VA_ARGS__)
#define efiPrintf(fmt, ...) efiPrintfProto(PROTOCOL_MSG, fmt, ##__VA_ARGS__)

// Base class of anything that can be handed over to logging central with loggingPostBuffer().
// The buffer is owned by whoever posted it: logging central only reads 'len' bytes out of it
// and then calls free(), which is where the owner recycles or reuses its storage
struct LogBufferBase {
	// number of bytes to transfer, to be set by the owner before posting
	size_t len = 0;

	// called by logging central once this buffer has been transferred
	virtual void free(void) = 0;
	virtual char* getBuffer() = 0;

	size_t used() {
		return len;
	}
};

// LogBufferBase with inline storage of the given size
template <size_t bufSize>
struct LogBuffer : public LogBufferBase {
	char buffer[bufSize];

	char* getBuffer() override {
		return buffer;
	}

	constexpr size_t size() {
		return bufSize;
	}
};

// Queue a filled buffer for transfer to TS, safe to call from ISR context
void loggingPostBuffer(LogBufferBase *buffer);
