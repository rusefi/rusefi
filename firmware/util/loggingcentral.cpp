/**
 * @file	loggingcentral.cpp
 *
 *
 * As of May 2019 we have given up on text-based 'push' terminal mode. At the moment binary protocol
 * is the consumen of this logging buffer.
 *
 * @date Mar 8, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "global.h"
#include "os_access.h"
#include "efilib.h"
#include "loggingcentral.h"

#if EFI_UNIT_TEST || EFI_SIMULATOR
extern bool verboseMode;
#endif /* EFI_UNIT_TEST */

#include "thread_controller.h"
#include "thread_priority.h"

struct LogLineBuffer {
	char buffer[128];
};

class LogBuffer {
public:
	static constexpr size_t bufferSize = DL_OUTPUT_BUFFER;

	void writeLine(LogLineBuffer* line) {
		writeInternal(line->buffer);
	}

	void writeLogger(Logging* logging) {
		writeInternal(logging->buffer);
	}

	size_t length() {
		return m_writePtr - buffer;
	}

	void reset() {
		m_writePtr = buffer;
		memset(buffer, 0, bufferSize);
	}

	const char* get() {
		return buffer;
	}

private:
	void writeInternal(const char* buffer) {
		size_t len = efiStrlen(buffer);
		size_t available = bufferSize - length();

		// If we can't fit the whole thing, write as much as we can
		len = minI(available, len);
		memcpy(m_writePtr, buffer, len);
		m_writePtr += len;
	}

	char buffer[bufferSize];
	char* m_writePtr = buffer;
};


chibios_rt::Mutex logBufferMutex;

LogBuffer buffers[2];
LogBuffer* currentBuffer = &buffers[0];
LogBuffer* backBuffer = &buffers[1];

/**
 * Actual communication layer invokes this method when it's ready to send some data out
 *
 * @return pointer to the buffer which should be print to console
 */
const char * swapOutputBuffers(int *actualOutputBufferSize) {
	{
		chibios_rt::MutexLocker lock(logBufferMutex);

		// Swap buffers under lock
		auto temp = currentBuffer;
		currentBuffer = backBuffer;
		backBuffer = temp;

		// Reset the front buffer - it's now empty
		currentBuffer->reset();
	}

	*actualOutputBufferSize = backBuffer->length();
#if EFI_ENABLE_ASSERTS
	size_t expectedOutputSize = efiStrlen(backBuffer->get());

	if (*actualOutputBufferSize != expectedOutputSize) {
		firmwareError(ERROR_LOGGING_SIZE_CALC, "lsize mismatch %d vs strlen %d", *actualOutputBufferSize, expectedOutputSize);

		return nullptr;
	}
#endif /* EFI_ENABLE_ASSERTS */
	return backBuffer->get();
}

constexpr size_t lineBufferCount = 32;

static LogLineBuffer lineBuffers[lineBufferCount];

// freeBuffers contains a queue of buffers that are not in use
chibios_rt::Mailbox<LogLineBuffer*, lineBufferCount> freeBuffers;
// filledBuffers contains a queue of buffers currently waiting to be written to the output buffer
chibios_rt::Mailbox<LogLineBuffer*, lineBufferCount> filledBuffers;

class LoggingBufferFlusher : public ThreadController<256> {
public:
	LoggingBufferFlusher() : ThreadController("lbf", PRIO_TEXT_LOG) { }

	void ThreadTask() override {
		while (true) {
			// Fetch a queued message
			LogLineBuffer* line;
			msg_t msg = filledBuffers.fetch(&line, TIME_INFINITE);

			if (msg == MSG_RESET) {
				// todo?
			} else {
				// Lock the buffer mutex - inhibit buffer swaps while writing
				chibios_rt::MutexLocker lock(logBufferMutex);

				// Write the line out to the output buffer
				currentBuffer->writeLine(line);

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

	lbf.Start();
}

namespace priv
{
/**
 * rusEfi business logic invokes this method in order to eventually print stuff to rusEfi console
 *
 * this whole method is executed under syslock so that we can have multiple threads use the same shared buffer
 * in order to reduce memory usage
 *
 * this is really 'global lock + printf + scheduleLogging + unlock' a bit more clear
 */

void scheduleMsgInternal(const char *format, ...) {
#if EFI_UNIT_TEST || EFI_SIMULATOR
	if (verboseMode) {
		va_list ap;
		va_start(ap, format);
		vprintf(format, ap);
		va_end(ap);
		printf("\r\n");
	}
#else
	for (unsigned int i = 0; i < strlen(format); i++) {
		// todo: open question which layer would not handle CR/LF properly?
		efiAssertVoid(OBD_PCM_Processor_Fault, format[i] != '\n', "No CRLF please");
	}
#if EFI_TEXT_LOGGING

	LogLineBuffer* lineBuffer;
	msg_t msg;

	{
		chibios_rt::CriticalSectionLocker csl;
		msg = freeBuffers.fetchI(&lineBuffer);
	}

	// No free buffers available, so we can't log
	if (msg != MSG_OK) {
		return;
	}

	va_list ap;
	va_start(ap, format);
	// Write the formatted string to the output buffer
	chvsnprintf(lineBuffer->buffer, sizeof(lineBuffer->buffer), format, ap);
	va_end(ap);

	{
		chibios_rt::CriticalSectionLocker csl;
		// Push the buffer in to the written list

		if ((void*)lineBuffer == (void*)&filledBuffers) {
			__asm volatile("BKPT #0\n");
		}

		filledBuffers.postI(lineBuffer);
	}

	// TODO: how do we detect if we need to call chSchRescheduleS()?
	if (!((ch.dbg.isr_cnt != (cnt_t)0) || (ch.dbg.lock_cnt <= (cnt_t)0))) {
		//chSchRescheduleS();
	}
#endif /* EFI_TEXT_LOGGING */
#endif /* EFI_UNIT_TEST */
}
} // namespace priv

/**
 * This method appends the content of specified thread-local logger into the global buffer
 * of logging content.
 * 
 * This is a legacy function, most normal logging should use scheduleMsg
 */
void scheduleLogging(Logging *logging) {
	// Lock the buffer mutex - inhibit buffer swaps while writing
	chibios_rt::MutexLocker lock(logBufferMutex);

	currentBuffer->writeLogger(logging);
}
