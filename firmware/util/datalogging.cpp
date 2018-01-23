/**
 * @file    datalogging.cpp
 * @brief   Buffered console output stream code
 *
 * Here we have a memory buffer and method related to
 * printing messages into this buffer. The purpose of the
 * buffer is to allow fast, non-blocking, thread-safe logging.
 *
 * The idea is that each interrupt handler would have it's own logging buffer. You can add
 * stuff into this buffer without any locking since it's you own buffer, and once you get
 * the whole message you invoke the scheduleLogging() method which appends your local content
 * into the global logging buffer, from which it is later dispatched to the console by our
 * main console thread.
 *
 * @date Feb 25, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdbool.h>
#include "main.h"

#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)
#include "chprintf.h"
#include "chmtx.h"
#include "memstreams.h"
#include "console_io.h"
#include "rfiutil.h"
#include "loggingcentral.h"

static MemoryStream intermediateLoggingBuffer;
static uint8_t intermediateLoggingBufferData[INTERMEDIATE_LOGGING_BUFFER_SIZE] CCM_OPTIONAL;
//todo define max-printf-buffer
static bool intermediateLoggingBufferInited = false;

/**
 * @returns true if data does not fit into this buffer
 */
static ALWAYS_INLINE bool validateBuffer(Logging *logging, const char *text, uint32_t extraLen) {
	if (logging->buffer == NULL) {
		firmwareError(CUSTOM_ERR_LOGGING_NOT_READY, "Logging not initialized: %s", logging->name);
		return true;
	}

	if (remainingSize(logging) < extraLen + 1) {
#if EFI_PROD_CODE
		const char * msg = extraLen > 50 ? "(long)" : text;
		warning(CUSTOM_LOGGING_BUFFER_OVERFLOW, "output overflow %s %d [%s]", logging->name, extraLen, msg);
#endif /* EFI_PROD_CODE */
		return true;
	}
	return false;
}

void append(Logging *logging, const char *text) {
	efiAssertVoid(text != NULL, "append NULL");
	uint32_t extraLen = efiStrlen(text);
	bool isCapacityProblem = validateBuffer(logging, text, extraLen);
	if (isCapacityProblem) {
		return;
	}
	strcpy(logging->linePointer, text);
	/**
	 * And now we are pointing at the zero char at the end of the buffer again
	 */
	logging->linePointer += extraLen;
}

/**
 * @note This method if fast because it does not validate much, be sure what you are doing
 */
void appendFast(Logging *logging, const char *text) {
	register char *s;
	s = logging->linePointer;
	while ((*s++ = *text++) != 0)
		;
	logging->linePointer = s - 1;
}

// todo: look into chsnprintf once on Chibios 3
static void vappendPrintfI(Logging *logging, const char *fmt, va_list arg) {
	intermediateLoggingBuffer.eos = 0; // reset
	efiAssertVoid(getRemainingStack(chThdGetSelfX()) > 128, "lowstck#1b");
	chvprintf((BaseSequentialStream *) &intermediateLoggingBuffer, fmt, arg);
	intermediateLoggingBuffer.buffer[intermediateLoggingBuffer.eos] = 0; // need to terminate explicitly
	append(logging, (char *) intermediateLoggingBufferData);
}

/**
 * this method acquires system lock to guard the shared intermediateLoggingBuffer memory stream
 */
static void vappendPrintf(Logging *logging, const char *fmt, va_list arg) {
	efiAssertVoid(getRemainingStack(chThdGetSelfX()) > 128, "lowstck#5b");
	if (!intermediateLoggingBufferInited) {
		firmwareError(CUSTOM_ERR_BUFF_INIT_ERROR, "intermediateLoggingBufferInited not inited!");
		return;
	}
	int wasLocked = lockAnyContext();
	vappendPrintfI(logging, fmt, arg);
	if (!wasLocked) {
		unlockAnyContext();
	}
}

void appendPrintf(Logging *logging, const char *fmt, ...) {
	efiAssertVoid(getRemainingStack(chThdGetSelfX()) > 128, "lowstck#4");
	va_list ap;
	va_start(ap, fmt);
	vappendPrintf(logging, fmt, ap);
	va_end(ap);
}

void initLoggingExt(Logging *logging, const char *name, char *buffer, int bufferSize) {
	logging->name = name;
	logging->buffer = buffer;
	logging->bufferSize = bufferSize;
	resetLogging(logging);
	logging->isInitialized = true;
}

int isInitialized(Logging *logging) {
	return logging->isInitialized;
}

void debugInt(Logging *logging, const char *caption, int value) {
	append(logging, caption);
	append(logging, DELIMETER);
	appendPrintf(logging, "%d%s", value, DELIMETER);
}

void appendFloat(Logging *logging, float value, int precision) {
	/**
	 * todo: #1 this implementation is less than perfect
	 * todo: #2 The only way to avoid double promotion would probably be using *float instead of float
	 * See also http://stackoverflow.com/questions/5522051/printing-a-float-in-c-while-avoiding-variadic-parameter-promotion-to-double
	 */
	switch (precision) {
	case 1:
		appendPrintf(logging, "%.1f", value);
		break;
	case 2:
		appendPrintf(logging, "%.2f", value);
		break;
	case 3:
		appendPrintf(logging, "%.3f", value);
		break;
	case 4:
		appendPrintf(logging, "%.4f", value);
		break;
	case 5:
		appendPrintf(logging, "%.5f", value);
		break;
	case 6:
		appendPrintf(logging, "%.6f", value);
		break;

	default:
		appendPrintf(logging, "%.2f", value);
	}
}

void debugFloat(Logging *logging, const char *caption, float value, int precision) {
	append(logging, caption);
	append(logging, DELIMETER);

	appendFloat(logging, value, precision);
	append(logging, DELIMETER);
}

static char header[16];

/**
 * this method should invoked on the main thread only
 */
void printWithLength(char *line) {
	/**
	 * this is my way to detect serial port transmission errors
	 * following code is functionally identical to
	 *   print("line:%d:%s\r\n", len, line);
	 * but it is faster because it outputs the whole buffer, not single characters
	 * We need this optimization because when we output larger chunks of data like the wave_chart:
	 * When we work with actual hardware, it is faster to invoke 'chSequentialStreamWrite' for the
	 * whole buffer then to invoke 'chSequentialStreamPut' once per character.
	 */
	int len = efiStrlen(line);
	strcpy(header, "line:");
	char *p = header + efiStrlen(header);
	p = itoa10(p, len);
	*p++ = ':';
	*p++ = '\0';

	p = line;
	p += len;
	*p++ = '\r';
	*p++ = '\n';

	if (!isCommandLineConsoleReady())
		return;
	consoleOutputBuffer((const uint8_t *) header, strlen(header));
	consoleOutputBuffer((const uint8_t *) line, p - line);
}

void appendMsgPrefix(Logging *logging) {
	append(logging, "msg" DELIMETER);
}

void appendMsgPostfix(Logging *logging) {
	append(logging, DELIMETER);
}

void resetLogging(Logging *logging) {
	char *buffer = logging->buffer;
	if (buffer == NULL) {
		firmwareError(ERROR_NULL_BUFFER, "Null buffer: %s", logging->name);
		return;
	}
	logging->linePointer = buffer;
	logging->linePointer[0] = 0;
}

/**
 * This method would output a simple console message immediately.
 * This method should only be invoked on main thread because only the main thread can write to the console
 */
void printMsg(Logging *logger, const char *fmt, ...) {
	efiAssertVoid(getRemainingStack(chThdGetSelfX()) > 128, "lowstck#5o");
//	resetLogging(logging); // I guess 'reset' is not needed here?
	appendMsgPrefix(logger);

	va_list ap;
	va_start(ap, fmt);
	vappendPrintf(logger, fmt, ap);
	va_end(ap);

	append(logger, DELIMETER);
	printWithLength(logger->buffer);
	resetLogging(logger);
}

/**
 * this whole method is executed under syslock so that we can have multiple threads use the same shared buffer
 * in order to reduce memory usage
 */
void scheduleMsg(Logging *logging, const char *fmt, ...) {
	if (logging == NULL) {
		warning(CUSTOM_ERR_LOGGING_NULL, "logging NULL");
		return;
	}
	int wasLocked = lockAnyContext();
	resetLogging(logging); // todo: is 'reset' really needed here?
	appendMsgPrefix(logging);

	va_list ap;
	va_start(ap, fmt);
	vappendPrintf(logging, fmt, ap);
	va_end(ap);

	appendMsgPostfix(logging);
	scheduleLogging(logging);
	if (!wasLocked) {
		unlockAnyContext();
	}
}

uint32_t remainingSize(Logging *logging) {
	return logging->bufferSize - loggingSize(logging);
}

void initIntermediateLoggingBuffer(void) {
	initLoggingCentral();

	msObjectInit(&intermediateLoggingBuffer, intermediateLoggingBufferData, INTERMEDIATE_LOGGING_BUFFER_SIZE, 0);
	intermediateLoggingBufferInited = true;
}

#endif /* ! EFI_UNIT_TEST */

void Logging::baseConstructor() {
	name = NULL;
	buffer = NULL;
	linePointer = NULL;
	bufferSize = 0;
	isInitialized = false;
}

Logging::Logging() {
	baseConstructor();
}

Logging::Logging(char const *name, char *buffer, int bufferSize) {
	baseConstructor();
#if ! EFI_UNIT_TEST
	initLoggingExt(this, name, buffer, bufferSize);
#endif /* ! EFI_UNIT_TEST */
}

LoggingWithStorage::LoggingWithStorage(const char *name) : Logging(name, DEFAULT_BUFFER, sizeof(DEFAULT_BUFFER))   {
}
