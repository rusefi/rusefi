/**
 * @file	loggingcentral.cpp
 *
 * This file implements text logging.
 *
 * Uses a queue of buffers so that the expensive printf operation doesn't require exclusive access
 * (ie, global system lock) to log.  In the past there have been serious performance problems caused
 * by heavy logging on a low priority thread that blocks the rest of the system running (trigger errors, etc).
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


#include "thread_controller.h"

/* for isprint() */
#include <ctype.h>

template <size_t TBufferSize>
void LogBuffer<TBufferSize>::writeLine(LogLineBuffer* line) {
	writeInternal(line->buffer, sizeof(line->buffer));
}

template <size_t TBufferSize>
void LogBuffer<TBufferSize>:: writeLogger(Logging* logging) {
	writeInternal(logging->buffer, logging->bufferSize);
}

template <size_t TBufferSize>
size_t LogBuffer<TBufferSize>::length() const {
	return m_writePtr - m_buffer;
}

template <size_t TBufferSize>
void LogBuffer<TBufferSize>::reset() {
	m_writePtr = m_buffer;
	*m_writePtr = '\0';
}

template <size_t TBufferSize>
const char* LogBuffer<TBufferSize>::get() const {
	return m_buffer;
}

template <size_t TBufferSize>
void LogBuffer<TBufferSize>::writeInternal(const char* buffer, size_t maxLength) {
	// never read past maxLength - an unterminated source buffer must not walk
	// adjacent memory (on F7 with MPU guard pages that walk ends in a MemManage
	// fault and a reboot), see https://github.com/rusefi/rusefi/issues/10159
	const char* terminator = static_cast<const char*>(std::memchr(buffer, '\0', maxLength));
	size_t len = terminator ? static_cast<size_t>(terminator - buffer) : maxLength;
	// leave one byte extra at the end to guarantee room for a null terminator
	size_t available = TBufferSize - length() - 1;

	// If we can't fit the whole thing, write as much as we can
	len = minI(available, len);
	// Ensure the output buffer is always null terminated (in case we did a partial write)
	*(m_writePtr + len) = '\0';
	memcpy(m_writePtr, buffer, len);
	m_writePtr += len;
}

// for unit tests
template class LogBuffer<10>;
#if EFI_UNIT_TEST
// large enough to accept a whole unterminated LogLineBuffer
template class LogBuffer<300>;
#endif

namespace priv
{
size_t terminateLogLine(char* buffer, size_t bufferSize, size_t untruncatedLen) {
	if (untruncatedLen > bufferSize - 1) {
		// The message was truncated, losing its trailing LOG_DELIMITER framing -
		// re-add the delimiter as the last visible character. The null terminator at
		// buffer[bufferSize - 1] MUST survive: a longer message used to overwrite it
		// with the delimiter here, leaving the buffer unterminated and sending the
		// flusher's strlen off the end of the buffer - on massive Lua print() traffic
		// that walk reads into adjacent memory and can end in a fault and a reboot,
		// see https://github.com/rusefi/rusefi/issues/10159
		untruncatedLen = bufferSize - 1;
		buffer[untruncatedLen - 1] = LOG_DELIMITER[0];
		// chvsnprintf already null-terminates here, but do not rely on it
		buffer[untruncatedLen] = '\0';
	}

	return untruncatedLen;
}
} // namespace priv

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
	size_t expectedOutputSize = std::strlen(readBuffer->get());

	// Check that the actual length of the buffer matches the expected length of how much we thought we wrote
	if (*actualOutputBufferSize != expectedOutputSize) {
		firmwareError(ObdCode::ERROR_LOGGING_SIZE_CALC, "lsize mismatch %d vs strlen %d", *actualOutputBufferSize, expectedOutputSize);

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

class LoggingBufferFlusher : public ThreadController<UTILITY_THREAD_STACK_SIZE> {
public:
	LoggingBufferFlusher() : ThreadController("log flush", PRIO_TEXT_LOG) { }

	void ThreadTask() override {
		while (true) {
			// Fetch a queued message
			LogLineBuffer* line;
			msg_t msg = filledBuffers.fetch(&line, TIME_INFINITE);

			if (msg != MSG_OK) {
				// This should be impossible - neither timeout or reset should happen
			} else {
				{
					// Lock the buffer mutex - inhibit buffer swaps while writing
					chibios_rt::MutexLocker lock(logBufferMutex);

					// Write the line out to the output buffer
					writeBuffer->writeLine(line);
				}

				// Return this line buffer to the free list
				freeBuffers.post(line, TIME_INFINITE);
			}
		}
	}
};

RUSEFI_STACK_ROOT(LoggingBufferFlusher, ThreadTask);

static LoggingBufferFlusher lbf;

void startLoggingProcessor() {
	// Push all buffers in to the free queue
	for (size_t i = 0; i < lineBufferCount; i++) {
		freeBuffers.post(&lineBuffers[i], TIME_INFINITE);
	}

	// Start processing used buffers
	lbf.start();
}

#endif // EFI_PROD_CODE

#if EFI_UNIT_TEST || EFI_SIMULATOR
extern bool verboseMode;
#endif

namespace priv
{
void efiPrintfInternal(const char *format, ...) {
#if EFI_UNIT_TEST || EFI_SIMULATOR
	/*
	 * Skip printf to stdout from ISR context — the C runtime's printf/vprintf
	 * can have large stack frames that overflow
	 * the ChibiOS thread working area into adjacent static globals, corrupting
	 * scheduler data structures
	 * ChibiOS suggested patch: https://github.com/rusefi/ChibiOS/pull/66
	 * ChibiOS issue: https://sourceforge.net/p/chibios/bugs/1305/
	 */
	if (verboseMode
#if EFI_SIMULATOR
		&& !port_is_isr_context()
#endif
	) {
		printf("[%dus]efiPrintfInternal:", time2print(getTimeNowUs()));
		va_list ap;
		va_start(ap, format);
		vprintf(format, ap);
		va_end(ap);
		printf("\r\n");
	}
#endif
#if (EFI_PROD_CODE || EFI_SIMULATOR) && EFI_TEXT_LOGGING
	LogLineBuffer* lineBuffer;
	msg_t msg;

	/*
	 * On the win32 simulator port, S-class chSysLock/chSysUnlock from ISR
	 * context corrupts the cooperative scheduler state (ready-list / delta-list)
	 * because port_unlock() resets port_irq_sts, breaking the lock invariant.
	 * On real ARM hardware the S-class and I-class locks are functionally
	 * identical (both just raise BASEPRI), so the S-class pattern works
	 */
#if EFI_SIMULATOR
	const bool isIsr = port_is_isr_context();
	if (isIsr) {
		chSysLockFromISR();
		msg = freeBuffers.fetchI(&lineBuffer);
		chSysUnlockFromISR();
	} else
#endif
	{
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
	// chvsnprintf always null-terminates (at most at buffer[size - 1]) and returns
	// the untruncated length
	size_t len = chvsnprintf(lineBuffer->buffer, sizeof(lineBuffer->buffer), format, ap);
	va_end(ap);

	len = priv::terminateLogLine(lineBuffer->buffer, sizeof(lineBuffer->buffer), len);
	for (size_t i = 0; i < len; i++) {
		/* just replace all non-printable chars with space
		 * TODO: is there any other "prohibited" chars? */
		if (isprint(lineBuffer->buffer[i]) == 0)
			lineBuffer->buffer[i] = ' ';
	}

#if EFI_SIMULATOR
	if (isIsr) {
		chSysLockFromISR();
		filledBuffers.postI(lineBuffer);
		chSysUnlockFromISR();
	} else
#endif
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
