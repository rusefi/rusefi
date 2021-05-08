/**
 * @file    datalogging.cpp
 * @brief   Buffered console output stream code
 *
 * Here we have a memory buffer and methods related to
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
 * @author Andrey Belomutskiy, (c) 2012-2020
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

#include "globalaccess.h"

#if ! EFI_UNIT_TEST
#include "os_access.h"
#include "chmtx.h"
#include "memstreams.h"
#include "console_io.h"
#include "os_util.h"
#endif // EFI_UNIT_TEST

static uint8_t intermediateLoggingBufferData[INTERMEDIATE_LOGGING_BUFFER_SIZE] CCM_OPTIONAL;

class IntermediateLogging {
public:
	/**
	 * Class constructors are a great way to have simple initialization sequence
	 */
	IntermediateLogging() {
#if ! EFI_UNIT_TEST
		msObjectInit(&intermediateLoggingBuffer, intermediateLoggingBufferData, INTERMEDIATE_LOGGING_BUFFER_SIZE, 0);
#endif // EFI_UNIT_TEST
	}
#if ! EFI_UNIT_TEST
	MemoryStream intermediateLoggingBuffer;
#endif // EFI_UNIT_TEST

	// todo: look into chsnprintf once on Chibios 3
	void vappendPrintfI(Logging *logging, const char *fmt, va_list arg) {
#if ! EFI_UNIT_TEST
		intermediateLoggingBuffer.eos = 0; // reset
		efiAssertVoid(CUSTOM_ERR_6603, getCurrentRemainingStack() > 128, "lowstck#1b");
		chvprintf((BaseSequentialStream *) &intermediateLoggingBuffer, fmt, arg);
		intermediateLoggingBuffer.buffer[intermediateLoggingBuffer.eos] = 0; // need to terminate explicitly
		logging->append((char *)intermediateLoggingBuffer.buffer);
#endif // EFI_UNIT_TEST
	}
};

static IntermediateLogging intermediateLogging;

/**
 * @returns true if data does not fit into this buffer
 */
bool Logging::validateBuffer(const char *text, uint32_t extraLen) {
	if (remainingSize() < extraLen + 1) {
#if EFI_PROD_CODE
		warning(CUSTOM_LOGGING_BUFFER_OVERFLOW, "output overflow %s %d", name, extraLen);
#endif /* EFI_PROD_CODE */
		return true;
	}

	return false;
}

void Logging::append(const char *text) {
	efiAssertVoid(CUSTOM_APPEND_NULL, text != NULL, "append NULL");
	uint32_t extraLen = efiStrlen(text);
	bool isCapacityProblem = validateBuffer(text, extraLen);
	if (isCapacityProblem) {
		return;
	}
	strcpy(linePointer, text);
	/**
	 * And now we are pointing at the zero char at the end of the buffer again
	 */
	linePointer += extraLen;
}

/**
 * @note This method if fast because it does not validate much, be sure what you are doing
 */
void Logging::appendFast(const char *text) {
	char *s = linePointer;
	while ((*s++ = *text++) != 0)
		;
	linePointer = s - 1;
}

/**
 * this method acquires system lock to guard the shared intermediateLoggingBuffer memory stream
 */
void Logging::vappendPrintf(const char *fmt, va_list arg) {
#if ! EFI_UNIT_TEST
#if EFI_ENABLE_ASSERTS
	// todo: Kinetis needs real getCurrentRemainingStack or mock
	if (getCurrentRemainingStack() < 128) {
		firmwareError(CUSTOM_ERR_6604, "lowstck#5b %s", chThdGetSelfX()->name);
	}
#endif // EFI_ENABLE_ASSERTS
	chibios_rt::CriticalSectionLocker csl;
	intermediateLogging.vappendPrintfI(this, fmt, arg);
#endif // EFI_UNIT_TEST
}

void Logging::appendPrintf(const char *fmt, ...) {
#if EFI_UNIT_TEST
	va_list ap;
	va_start(ap, fmt);
	vsprintf(buffer, fmt, ap);
	va_end(ap);
#else
	efiAssertVoid(CUSTOM_APPEND_STACK, getCurrentRemainingStack() > 128, "lowstck#4");
	va_list ap;
	va_start(ap, fmt);
	vappendPrintf(fmt, ap);
	va_end(ap);
#endif // EFI_UNIT_TEST
}

void Logging::appendFloat(float value, int precision) {
	/**
	 * todo: #1 this implementation is less than perfect
	 * todo: #2 The only way to avoid double promotion would probably be using *float instead of float
	 * See also http://stackoverflow.com/questions/5522051/printing-a-float-in-c-while-avoiding-variadic-parameter-promotion-to-double
	 */
	switch (precision) {
	case 1:
		appendPrintf("%.1f", value);
		break;
	case 2:
		appendPrintf("%.2f", value);
		break;
	case 3:
		appendPrintf("%.3f", value);
		break;
	case 4:
		appendPrintf("%.4f", value);
		break;
	case 5:
		appendPrintf("%.5f", value);
		break;
	case 6:
		appendPrintf("%.6f", value);
		break;

	default:
		appendPrintf("%.2f", value);
	}
}

void appendMsgPrefix(Logging *logging) {
	logging->append(PROTOCOL_MSG DELIMETER);
}

void appendMsgPostfix(Logging *logging) {
	logging->append(DELIMETER);
}

void Logging::reset() {
	linePointer = buffer;
	*linePointer = 0;
}

Logging::Logging(char const *name, char *buffer, int bufferSize)
	: name(name)
	, buffer(buffer)
	, bufferSize(bufferSize)
{
	reset();
}

LoggingWithStorage::LoggingWithStorage(const char *name) : Logging(name, DEFAULT_BUFFER, sizeof(DEFAULT_BUFFER))   {
}
