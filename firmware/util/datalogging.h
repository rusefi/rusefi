/**
 * @file    datalogging.h
 * @brief   Buffered console output stream header
 *
 * @date Feb 25, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
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
	void baseConstructor();
	Logging();
	Logging(const char *name, char *buffer, int bufferSize);
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
	volatile bool isInitialized;
};

class LoggingWithStorage : public Logging {
public:
	LoggingWithStorage(const char *name);
	char DEFAULT_BUFFER[200];
};

void initIntermediateLoggingBuffer(void);

int isInitialized(Logging *logging);

void initLoggingExt(Logging *logging, const char *name, char *buffer, int bufferSize);

void debugInt(Logging *logging, const char *caption, int value);

void debugFloat(Logging *logging, const char *text, float value, int precision);
void appendFloat(Logging *logging, float value, int precision);

void resetLogging(Logging *logging);

void appendMsgPrefix(Logging *logging);
void appendMsgPostfix(Logging *logging);

void scheduleMsg(Logging *logging, const char *fmt, ...);


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define lockOutputBuffer lockAnyContext
#define unlockOutputBuffer unlockAnyContext

uint32_t remainingSize(Logging *logging);

#define loggingSize(logging) ((int) (logging)->linePointer - (int) ((logging)->buffer))


void printMsg(Logging *logging, const char *fmt, ...);
void appendPrintf(Logging *logging, const char *fmt, ...);
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

void printWithLength(char *line);

#endif /* DATALOGGING_H_ */
