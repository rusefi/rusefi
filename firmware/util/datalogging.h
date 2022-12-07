/**
 * @file    datalogging.h
 * @brief   Buffered console output stream header
 *
 * @date Feb 25, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include <cstdarg>
#include <cstdint>
#include <cstddef>

// todo: migrate to external buffer so that different instances have different
// size of buffers?
class Logging {
public:
	Logging() = delete;
	Logging(const char *name, char *buffer, int bufferSize);

	void reset();

	void append(const char *text);
	void appendFast(const char *text);
	void appendPrintf(const char *fmt, ...);
	void appendFloat(float value, int precision);

	void terminate() {
		linePointer[0] = '\0';
	}

	/**
	 * This macro breaks the normal zero=termination constraint, please take care of this outside of this function
	 */
	void appendChar(char c) {
		*linePointer = c;
		linePointer++;
	}

	size_t loggingSize() const {
		return (uintptr_t)linePointer - (uintptr_t)buffer;
	}

	size_t remainingSize() const {
		return bufferSize - loggingSize();
	}

//private:
	bool validateBuffer(uint32_t extraLen);

	const char* const name = nullptr;

	/**
	 * Zero-terminated buffer of pending debug message
	 *
	 * Unless a larger external buffer is specified, this is just a pointer to DEFAULT_BUFFER
	 */
	char* const buffer = nullptr;
	const int bufferSize = 0;

	/**
	 * This pointer is always pointing at the position within the buffer into which next
	 * write operation would append additional data
	 */
	char *linePointer = nullptr;
};

class LoggingWithStorage : public Logging {
public:
	explicit LoggingWithStorage(const char *name);
	char DEFAULT_BUFFER[100];
};

void initLoggingExt(Logging *logging, const char *name, char *buffer, int bufferSize);
