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
		m_linePointer[0] = '\0';
	}

	/**
	 * This macro breaks the normal zero=termination constraint, please take care of this outside of this function
	 */
	void appendChar(char c) {
		*m_linePointer = c;
		m_linePointer++;
	}

	size_t loggingSize() const {
		return (uintptr_t)m_linePointer - (uintptr_t)m_buffer;
	}

	size_t remainingSize() const {
		return m_bufferSize - loggingSize();
	}

//private:
	bool validateBuffer(uint32_t extraLen);

	const char* const m_name;

	/**
	 * Zero-terminated buffer of pending debug message
	 *
	 * Unless a larger external buffer is specified, this is just a pointer to DEFAULT_BUFFER
	 */
	char* const m_buffer;
	const int m_bufferSize;

	/**
	 * This pointer is always pointing at the position within the buffer into which next
	 * write operation would append additional data
	 */
	char* m_linePointer = nullptr;
};

class LoggingWithStorage : public Logging {
public:
	explicit LoggingWithStorage(const char *name);
	char DEFAULT_BUFFER[100];
};

void initLoggingExt(Logging *logging, const char *name, char *buffer, int bufferSize);
