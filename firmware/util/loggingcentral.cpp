/**
 * @file	loggingcentral.cpp
 *
 * This file implements text logging.
 * 
 * Uses a queue of buffers so that the expensive printf operation doesn't require exclusive access
 * (ie, global system lock) to log.  In the past there have been serious performance problems caused
 * by heavy logging on a low prioriy thread that blocks the rest of the system running (trigger errors, etc).
 * 
 * Uses ChibiOS message queues to maintain one queue of free buffers, and one queue of used buffers.
 * When a thread wants to write, it acquires a free buffer, prints to it, and pushes it in to the
 * used queue. A dedicated thread then dequeues and writes lines from the used buffer in to the
 * large output buffer.
 * 
 * Later, the binary TS thread will request access to the output log buffer for reading, so a lock is taken,
 * buffers, swapped, and the back buffer returned.  This blocks neither output nor logging in any case, as
 * each operation operates on a different buffer.
 *
 * @date Mar 8, 2015, heavily revised April 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 * @author Matthew Kennedy
 */

#include "pch.h"

#include "os_access.h"
#include "thread_controller.h"

template <size_t TBufferSize>
void LogBuffer<TBufferSize>::writeLine(LogLineBuffer* line) {
	writeInternal(line->buffer);
}

template <size_t TBufferSize>
void LogBuffer<TBufferSize>:: writeLogger(Logging* logging) {
	writeInternal(logging->buffer);
}

template <size_t TBufferSize>
size_t LogBuffer<TBufferSize>::length() const {
	return m_writePtr - m_buffer;
}

template <size_t TBufferSize>
void LogBuffer<TBufferSize>::reset() {
	m_writePtr = m_buffer;
	memset(m_buffer, 0, TBufferSize);
}

template <size_t TBufferSize>
const char* LogBuffer<TBufferSize>::get() const {
	return m_buffer;
}

template <size_t TBufferSize>
void LogBuffer<TBufferSize>::writeInternal(const char* buffer) {
	size_t len = efiStrlen(buffer);
	// leave one byte extra at the end to guarantee room for a null terminator
	size_t available = TBufferSize - length() - 1;

	// If we can't fit the whole thing, write as much as we can
	len = minI(available, len);
	memcpy(m_writePtr, buffer, len);
	m_writePtr += len;

	// Ensure the output buffer is always null terminated (in case we did a partial write)
	*m_writePtr = '\0';
}

// for unit tests
template class LogBuffer<10>;

#if (EFI_PROD_CODE || EFI_SIMULATOR) && EFI_TEXT_LOGGING

// This mutex protects the LogBuffer instances below
chibios_rt::Mutex logBufferMutex;

// Two buffers:
//  - we copy line buffers to writeBuffer in LoggingBufferFlusher
//  - and read from readBuffer via TunerStudio protocol commands
using LB = LogBuffer<DL_OUTPUT_BUFFER>;
LB buffers[2];
LB* writeBuffer = &buffers[0];
LB* readBuffer = &buffers[1];

/**
 * Actual communication layer invokes this method when it's ready to send some data out
 *
 * @return pointer to the buffer which should be print to console
 */
const char* swapOutputBuffers(size_t* actualOutputBufferSize) {
	{
		chibios_rt::MutexLocker lock(logBufferMutex);

		// Swap buffers under lock
		auto temp = writeBuffer;
		writeBuffer = readBuffer;
		readBuffer = temp;

		// Reset the front buffer - it's now empty
		writeBuffer->reset();
	}

	*actualOutputBufferSize = readBuffer->length();
#if EFI_ENABLE_ASSERTS
	size_t expectedOutputSize = efiStrlen(readBuffer->get());

	// Check that the actual length of the buffer matches the expected length of how much we thought we wrote
	if (*actualOutputBufferSize != expectedOutputSize) {
		firmwareError(ERROR_LOGGING_SIZE_CALC, "lsize mismatch %d vs strlen %d", *actualOutputBufferSize, expectedOutputSize);

		return nullptr;
	}
#endif /* EFI_ENABLE_ASSERTS */
	return readBuffer->get();
}

// These buffers store lines queued to be written to the writeBuffer
constexpr size_t lineBufferCount = 24;
static LogLineBuffer lineBuffers[lineBufferCount];

// freeBuffers contains a queue of buffers that are not in use
static chibios_rt::Mailbox<LogLineBuffer*, lineBufferCount> freeBuffers;
// filledBuffers contains a queue of buffers currently waiting to be written to the output buffer
static chibios_rt::Mailbox<LogLineBuffer*, lineBufferCount> filledBuffers;

class LoggingBufferFlusher : public ThreadController<256> {
public:
	LoggingBufferFlusher() : ThreadController("log flush", PRIO_TEXT_LOG) { }

	void ThreadTask() override {
		while (true) {
			// Fetch a queued message
			LogLineBuffer* line;
			msg_t msg = filledBuffers.fetch(&line, TIME_INFINITE);

			if (msg == MSG_RESET) {
				// todo?
				// what happens if MSG_RESET?
			} else {
				// Lock the buffer mutex - inhibit buffer swaps while writing
				chibios_rt::MutexLocker lock(logBufferMutex);

				// Write the line out to the output buffer
				writeBuffer->writeLine(line);

				// Return this line buffer to the free list
				freeBuffers.post(line, TIME_INFINITE);
			}
		}
	}
};

static LoggingBufferFlusher lbf;

void startLoggingProcessor() {
	// Push all buffers in to the free queue
	for (size_t i = 0; i < lineBufferCount; i++) {
		freeBuffers.post(&lineBuffers[i], TIME_INFINITE);
	}

	// Start processing used buffers
	lbf.Start();
}

#endif // EFI_PROD_CODE

#if EFI_UNIT_TEST || EFI_SIMULATOR
extern bool verboseMode;
#endif

namespace priv
{
void efiPrintfInternal(const char *format, ...) {
#if EFI_UNIT_TEST || EFI_SIMULATOR
	if (verboseMode) {
		va_list ap;
		va_start(ap, format);
		vprintf(format, ap);
		va_end(ap);
		printf("\r\n");
	}
#endif
#if (EFI_PROD_CODE || EFI_SIMULATOR) && EFI_TEXT_LOGGING
	for (unsigned int i = 0; i < strlen(format); i++) {
		// todo: open question which layer would not handle CR/LF properly?
		efiAssertVoid(OBD_PCM_Processor_Fault, format[i] != '\n', "No CRLF please");
	}

	LogLineBuffer* lineBuffer;
	msg_t msg;

	{
		// Acquire a buffer we can write to
		chibios_rt::CriticalSectionLocker csl;
		msg = freeBuffers.fetchI(&lineBuffer);
	}

	// No free buffers available, so we can't log
	if (msg != MSG_OK) {
		return;
	}

	// Write the formatted string to the output buffer
	va_list ap;
	va_start(ap, format);
	chvsnprintf(lineBuffer->buffer, sizeof(lineBuffer->buffer), format, ap);
	va_end(ap);

	// Ensure that the string is comma-terminated in case it overflowed
	lineBuffer->buffer[sizeof(lineBuffer->buffer) - 1] = DELIMETER;

	{
		// Push the buffer in to the written list so it can be written back
		chibios_rt::CriticalSectionLocker csl;

		filledBuffers.postI(lineBuffer);
	}
#endif
}
} // namespace priv

/**
 * This method appends the content of specified thread-local logger into the global buffer
 * of logging content.
 * 
 * This is a legacy function, most normal logging should use efiPrintf
 */
void scheduleLogging(Logging *logging) {
#if (EFI_PROD_CODE || EFI_SIMULATOR) && EFI_TEXT_LOGGING
	// Lock the buffer mutex - inhibit buffer swaps while writing
	{
		chibios_rt::MutexLocker lock(logBufferMutex);

		writeBuffer->writeLogger(logging);
	}

	// Reset the logging now that it's been written out
	logging->reset();
#endif
}
