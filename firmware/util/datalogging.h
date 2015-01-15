/**
 * @file    datalogging.h
 * @brief   Buffered console output stream header
 *
 * @date Feb 25, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef DATALOGGING_H_
#define DATALOGGING_H_

#include "global.h"
#include <stdarg.h>

#define DELIMETER	","

// todo: migrate to external buffer so that different instances have different
// size of buffers?
class Logging {
public:
	const char *name;
	char SMALL_BUFFER[40];
	/**
	 * Zero-terminated buffer of pending debug message
	 *
	 * Unless a larger external buffer is specified, this is just a pointer to DEFAULT_BUFFER
	 */
	char *buffer;
	/**
	 * This pointer is always pointing at the position within the buffer into which next
	 * write operation would append additional data
	 */
	char *linePointer;
	int bufferSize;
	volatile int isInitialized;
};

class LoggingWithStorage : public Logging {
public:
	LoggingWithStorage();
	char DEFAULT_BUFFER[200];
};

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

bool lockOutputBuffer(void);
void unlockOutputBuffer(void);

void initIntermediateLoggingBuffer(void);
uint32_t remainingSize(Logging *logging);

#define loggingSize(logging) ((int) (logging)->linePointer - (int) ((logging)->buffer))

int isInitialized(Logging *logging);

void initLogging(LoggingWithStorage *logging, const char *name);
void initLoggingExt(Logging *logging, const char *name, char *buffer, int bufferSize);

void debugInt(Logging *logging, const char *caption, int value);

void debugFloat(Logging *logging, const char *text, float value, int precision);
void appendFloat(Logging *logging, float value, int precision);

void resetLogging(Logging *logging);
void printLine(Logging *logging);

void appendMsgPrefix(Logging *logging);
void appendMsgPostfix(Logging *logging);

void scheduleMsg(Logging *logging, const char *fmt, ...);

void printMsg(Logging *logging, const char *fmt, ...);
void appendPrintf(Logging *logging, const char *fmt, ...);
void vappendPrintf(Logging *logging, const char *fmt, va_list arg);
void append(Logging *logging, const char *text);
void appendFast(Logging *logging, const char *text);

/**
 * This macro breaks the normal zero=termination constraint, please take care of this outside of this macro
 */
#define appendChar(logging, symbol) {(logging)->linePointer[0] = (symbol);(logging)->linePointer++;}

/**
 * this method copies the line into the intermediate buffer for later output by
 * the main thread
 */
void scheduleLogging(Logging *logging);

/**
 * this should only be invoked by the 'main' thread in order to keep the console safe
 */
void printPending(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DATALOGGING_H_ */
