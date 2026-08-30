/**
 * @file	loggingcentral.h
 *
 * @date Mar 8, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */
#pragma once

#include <cstddef>
#include "generated_lookup_meta.h"

class Logging;

void startLoggingProcessor();

const char* swapOutputBuffers(size_t *actualOutputBufferSize);

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
// Limits (see LogLineBuffer below): one efiPrintf call produces at most
// sizeof(LogLineBuffer::buffer) - 1 = 255 characters INCLUDING the
// PROTOCOL_MSG + LOG_DELIMITER framing; anything longer is truncated, not split.
// With the "msg" proto that leaves 245 characters for the formatted payload.
#define efiPrintfProto(proto, fmt, ...) priv::efiPrintfInternal(proto LOG_DELIMITER fmt LOG_DELIMITER, ##__VA_ARGS__)
#define efiPrintf(fmt, ...) efiPrintfProto(PROTOCOL_MSG, fmt, ##__VA_ARGS__)

/**
 * This is the legacy function to copy the contents of a local Logging object in to the output buffer
 */
void scheduleLogging(Logging *logging);

// Stores the result of one call to efiPrintfInternal in the queue to be copied out to the output buffer.
//
// Limits:
//  - 256 bytes per line: up to 255 visible characters plus the null terminator. A longer
//    efiPrintf result is TRUNCATED to 255 characters, the last visible one being replaced
//    by LOG_DELIMITER so the TS text framing survives (priv::terminateLogLine). Nothing is
//    split over several lines.
//  - lineBufferCount (24) of these are statically allocated (loggingcentral.cpp). When all
//    are waiting for the flusher thread, efiPrintf DROPS the line silently - a tight print()
//    loop (e.g. Lua) loses lines rather than blocking the caller.
//  - Do not assume buffer is null-terminated when reading it, see
//    https://github.com/rusefi/rusefi/issues/10159
struct LogLineBuffer {
	char buffer[256];
};

template <size_t TBufferSize>
class LogBuffer {
public:
	void writeLine(LogLineBuffer* line);
	void writeLogger(Logging* logging);

	size_t length() const;
	void reset();
	const char* get() const;

#if !EFI_UNIT_TEST
private:
#endif
	// maxLength caps how far the source buffer may be read: the source is not
	// trusted to be null-terminated, see https://github.com/rusefi/rusefi/issues/10159
	void writeInternal(const char* buffer, size_t maxLength);

	char m_buffer[TBufferSize];
	char* m_writePtr = m_buffer;
};
