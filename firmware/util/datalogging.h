/**
 * @file    datalogging.h
 * @brief   Buffered console output stream header
 *
 * @date Feb 25, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include <stdarg.h>
#include <stdint.h>

#define DELIMETER	","

// todo: migrate to external buffer so that different instances have different
// size of buffers?
class Logging {
public:
	Logging() = delete;
	Logging(const char *name, char *buffer, int bufferSize);

	void reset();

	void vappendPrintf(const char *fmt, va_list arg);
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
	bool validateBuffer(const char *text, uint32_t extraLen);

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
	char DEFAULT_BUFFER[200];
};

void initLoggingExt(Logging *logging, const char *name, char *buffer, int bufferSize);

void appendMsgPrefix(Logging *logging);
void appendMsgPostfix(Logging *logging);


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void printMsg(Logging *logging, const char *fmt, ...);

/**
 * this method copies the line into the intermediate buffer for later output by
 * the main thread
 */
void scheduleLogging(Logging *logging);

#ifdef __cplusplus
}
#endif /* __cplusplus */
