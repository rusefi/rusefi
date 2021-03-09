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
#include "thread_controller.h"
#include "thread_priority.h"

#if EFI_PROD_CODE

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

// This mutex protects the LogBuffer instances below
chibios_rt::Mutex logBufferMutex;

// Two buffers:
//  - we copy line buffers to writeBuffer in LoggingBufferFlusher
//  - and read from readBuffer via TunerStudio protocol commands
LogBuffer buffers[2];
LogBuffer* writeBuffer = &buffers[0];
LogBuffer* readBuffer = &buffers[1];

/**
 * Actual communication layer invokes this method when it's ready to send some data out
 *
 * @return pointer to the buffer which should be print to console
 */
const char * swapOutputBuffers(size_t* actualOutputBufferSize) {
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

namespace priv
{
void efiPrintfInternal(const char *format, ...) {
#if EFI_UNIT_TEST || EFI_SIMULATOR
	extern bool verboseMode;
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

	{
		// Push the buffer in to the written list so it can be written back
		chibios_rt::CriticalSectionLocker csl;

		if ((void*)lineBuffer == (void*)&filledBuffers) {
			__asm volatile("BKPT #0\n");
		}

		filledBuffers.postI(lineBuffer);
	}
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

	writeBuffer->writeLogger(logging);
}
