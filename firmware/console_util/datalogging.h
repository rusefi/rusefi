/**
 * @file    datalogging.h
 * @brief   Buffered console output stream header
 *
 * @date Feb 25, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef DATALOGGING_H_
#define DATALOGGING_H_

#include "global.h"
#include <stdarg.h>

#define DELIMETER	","

typedef enum {
	LP_RPM,
	LP_ECT,
	LP_IAT,
	LP_THROTTLE, LP_SECONDS,
	LP_MAP,
	LP_MAF,
	LP_MAP_RAW,

} LoggingPoints;

// todo: migrate to external buffer so that different instances have different
// size of buffers?
typedef struct {
	char *name;
	char SMALL_BUFFER[40];
	// todo: explicitly default buffer externally so that we do not have default_buffer where we do not need it?
	char DEFAULT_BUFFER[200];
	char *buffer;
	char *linePointer;
	int bufferSize;
	volatile int isInitialized;
} Logging;

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


void lockOutputBuffer(void);
void unlockOutputBuffer(void);

void initIntermediateLoggingBuffer(void);
uint32_t loggingSize(Logging *logging);

int isInitialized(Logging *logging);

void initLogging(Logging *logging, char *name);
void initLoggingExt(Logging *logging, char *name, char *buffer, int bufferSize);

void debugInt(Logging *logging, char *caption, int value);
void logInt(Logging *logging, LoggingPoints loggingPoint, int value);

void debugFloat(Logging *logging, char *text, float value, int precision);
void logFloat(Logging *logging, LoggingPoints loggingPoint, float value);
void appendFloat(Logging *logging, float value, int precision);

void resetLogging(Logging *logging);
void printLine(Logging *logging);

void appendMsgPrefix(Logging *logging);
void appendMsgPostfix(Logging *logging);

void scheduleMsg(Logging *logging, const char *fmt, ...);

void printMsg(Logging *logging, const char *fmt, ...);
char* getCaption(LoggingPoints loggingPoint);
void appendPrintf(Logging *logging, const char *fmt, ...);
void vappendPrintf(Logging *logging, const char *fmt, va_list arg);
void append(Logging *logging, char *text);
/**
 * this method copies the line into the intermediate buffer for later output by
 * the main thread
 */
 
void scheduleLogging(Logging *logging);

void scheduleIntValue(Logging *logging, char *msg, int value);

/**
 * this should only be invoked by the 'main' thread in order to keep the console safe
 */
void printPending(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DATALOGGING_H_ */
