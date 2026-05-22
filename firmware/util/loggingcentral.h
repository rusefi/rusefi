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

size_t loggingGetOutputData(char *buffer, size_t size);
size_t loggingSendOutputData(TsChannelBase* tsChannel);

namespace priv
{
	// internal implementation, use efiPrintf below
	void efiPrintfInternal(const char *fmt, ...)
		#if EFI_PROD_CODE
			__attribute__ ((format (printf, 1, 2)))
		#endif
			;
}

// "normal" logging messages need a header and footer, so put them in
// the format string at compile time
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
