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

#include "pch.h"

#if ! EFI_UNIT_TEST
#include "os_access.h"
#include "chmtx.h"
#include "memstreams.h"
#include "console_io.h"
#include "os_util.h"
#endif // EFI_UNIT_TEST

/**
 * @returns true if data does not fit into this buffer
 */
bool Logging::validateBuffer(uint32_t extraLen) {
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
	bool isCapacityProblem = validateBuffer(extraLen);
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

void Logging::appendPrintf(const char *fmt, ...) {
#if EFI_UNIT_TEST
	va_list ap;
	va_start(ap, fmt);
	vsprintf(buffer, fmt, ap);
	va_end(ap);
#else
	efiAssertVoid(CUSTOM_APPEND_STACK, getCurrentRemainingStack() > 128, "lowstck#4");

	size_t available = remainingSize();

	va_list ap;
	va_start(ap, fmt);
	size_t written = chvsnprintf(linePointer, available, fmt, ap);
	va_end(ap);

	// chvnsprintf returns how many bytes WOULD HAVE been written if it fit,
	// so clip it to the available space if necessary
	linePointer += (written > available) ? available : written;
	// ensure buffer is always null terminated
	buffer[bufferSize - 1] = '\0';

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
