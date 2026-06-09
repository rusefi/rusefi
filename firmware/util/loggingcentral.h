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

void startLoggingProcessor();

size_t loggingSendOutputData(TsChannelBase* tsChannel);

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

struct LogBufferBase {
	size_t len = 0;

	virtual void free(void) = 0;
	virtual char* getBuffer() = 0;

	size_t used() {
		return len;
	}
};

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

void loggingPostBuffer(LogBufferBase *buffer);

void scheduleLogging(Logging *logging);
