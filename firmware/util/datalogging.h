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
	LP_RPM = 0,
	LP_ECT = 1,
	LP_IAT = 2,

	LP_THROTTLE = 3,
	LP_THROTTLE_ADC = 4,

	LP_MAP = 5,
	LP_MAP_RAW = 6,

	LP_MAF = 7,
	LP_TRG_CH0_DUTY = 8,
	LP_TRG_CH1_DUTY = 9,


	//	LP_SECONDS,
	LP_COUNT = 9

} LoggingPoints;

// todo: migrate to external buffer so that different instances have different
// size of buffers?
typedef struct {
	const char *name;
	char SMALL_BUFFER[40];
	// todo: explicitly default buffer externally so that we do not have default_buffer where we do not need it?
	char DEFAULT_BUFFER[200];
	/**
	 * Zero-terminated buffer of pending debug message
	 *
	 * Unless a larger exteran buffer is specified, this is just a pointer to DEFAULT_BUFFER
	 */
	char *buffer;
	/**
	 * This pointer is always pointing at the position within the buffer into which next
	 * write operation would append additional data
	 */
	char *linePointer;
	int bufferSize;
	volatile int isInitialized;
} Logging;

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

void initLogging(Logging *logging, const char *name);
void initLoggingExt(Logging *logging, const char *name, char *buffer, int bufferSize);

void debugInt(Logging *logging, const char *caption, int value);
void logInt(Logging *logging, LoggingPoints loggingPoint, int value);

void debugFloat(Logging *logging, const char *text, float value, int precision);
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
