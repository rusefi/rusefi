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

/* for isprint() */
#include <ctype.h>

#if (EFI_PROD_CODE || EFI_SIMULATOR) && EFI_TEXT_LOGGING

// Stores the result of one call to efiPrintfInternal in the queue to be copied out to the output buffer
struct LogLineBuffer : LogBuffer<128>{
	void free(void) override;
};

// These buffers store lines
constexpr size_t lineBufferCount = 64;
static LogLineBuffer lineBuffers[lineBufferCount];

// freeBuffers contains a queue of line buffers that are not in use
static chibios_rt::Mailbox<LogLineBuffer*, lineBufferCount> freeBuffers;
// filledBuffers contains a queue of buffers currently waiting to be transfered
static chibios_rt::Mailbox<LogBufferBase*, lineBufferCount + 10> filledBuffers;

void LogLineBuffer::free() {
	freeBuffers.post(this, TIME_INFINITE);
}

/**
 * Fill buffer with as much data as possible
 *
 * @return actual size to transfer
 */
size_t loggingGetOutputData(char *buffer, size_t size) {
	LogBufferBase* line;
	size_t offset = 0;
	// TODO: better?
	while (offset + 128 <= size) {
		// Fetch a queued message
		msg_t msg = filledBuffers.fetch(&line, TIME_IMMEDIATE);

		if (msg != MSG_OK) {
			return offset;
		}

		const char* content = line->getBuffer();
		size_t len = std::strlen(content);
		memcpy(buffer + offset, content, len);
		offset += len;

		// Return this logging buffer to the owner
		line->free();
	}

	return offset;
}

// actually we can send much more in one packed. this is just a threshold that triggers current packet finalization
constexpr size_t maxSend = scratchBuffer_SIZE;

size_t loggingSendOutputData(TsChannelBase* tsChannel) {
	// consolidate up to 16 line buffers in one TS packet
	LogBufferBase* bufs[16];
	size_t totalBufs = 0;
	size_t totalSize = 0;

	while ((totalSize < maxSend) && (totalBufs < efi::size(bufs))) {
		// Fetch a queued message
		msg_t msg = filledBuffers.fetch(&bufs[totalBufs], TIME_IMMEDIATE);

		if (msg != MSG_OK) {
			break;
		}

		totalSize += bufs[totalBufs]->used();
		totalBufs++;
	}

	uint32_t crc = tsChannel->writePacketHeader(TS_RESPONSE_OK, totalSize);

	for (size_t i = 0; i < totalBufs; i++) {
		LogBufferBase *buf = bufs[i];

		crc = tsChannel->writePacketBody((uint8_t *)buf->getBuffer(), buf->used(), crc);
		buf->free();
	}

	tsChannel->writeCrcPacketTail(crc);

	return totalSize;
}

void startLoggingProcessor() {
	// Push all buffers in to the free queue
	for (size_t i = 0; i < lineBufferCount; i++) {
		LogLineBuffer* line = &lineBuffers[i];

		line->len = 0;
		line->free();
	}
}

#endif // EFI_PROD_CODE

#if EFI_UNIT_TEST || EFI_SIMULATOR
extern bool verboseMode;
#endif

size_t maxPrintfSize = 0;

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
	size_t len = chvsnprintf(lineBuffer->buffer, sizeof(lineBuffer->buffer), format, ap);
	va_end(ap);

	// Ensure that the string is comma-terminated in case it overflowed
	lineBuffer->buffer[sizeof(lineBuffer->buffer) - 1] = LOG_DELIMITER[0];

	if (len > sizeof(lineBuffer->buffer) - 1)
		len = sizeof(lineBuffer->buffer) - 1;
	for (size_t i = 0; i < len; i++) {
		/* just replace all non-printable chars with space
		 * TODO: is there any other "prohibited" chars? */
		if (isprint(lineBuffer->buffer[i]) == 0)
			lineBuffer->buffer[i] = ' ';
	}
	lineBuffer->len = len;

	if (len > maxPrintfSize) {
		maxPrintfSize = len;
	}

	loggingPostBuffer(lineBuffer);
#endif
}
} // namespace priv

void loggingPostBuffer(LogBufferBase *buffer) {
#if EFI_SIMULATOR
	const bool isIsr = port_is_isr_context();
	if (isIsr) {
		chSysLockFromISR();
		filledBuffers.postI(buffer);
		chSysUnlockFromISR();
	} else
#endif
	{
		// Push the buffer in to the written list so it can be written back
		chibios_rt::CriticalSectionLocker csl;
		filledBuffers.postI(buffer);
	}
}

/**
 * This method appends the content of specified thread-local logger into the global buffer
 * of logging content.
 *
 * This is a legacy function, most normal logging should use efiPrintf
 */
void scheduleLogging(Logging *logging) {

#if 0
#if (EFI_PROD_CODE || EFI_SIMULATOR) && EFI_TEXT_LOGGING
	LogBufferBase* lineBase;
	chibios_rt::CriticalSectionLocker csl;

	cnt_t free = freeBuffers.getUsedCountI();
	if (free * (128 - 1) >= logging->loggingSize()) {
		msg_t msg;
		const char *writePrt = logging->buffer;

		while (writePrt < logging->linePointer) {
			msg = freeBuffers.fetchI(&lineBase);
			if (msg != MSG_OK) {
				break;
			}
			LogLineBuffer* lineBuffer = static_cast<LogLineBuffer*>(lineBase);

			size_t len = minI(128 - 1, logging->linePointer - writePrt);
			memcpy(lineBuffer->buffer, writePrt, len);
			lineBuffer->buffer[len] = '\0';
			lineBuffer->len = len;
			writePrt += len;
			filledBuffers.postI(lineBuffer);
		}
	} else {
		// drop whole Logging if not enought line buffers available
	}


	// Lock the buffer mutex - inhibit buffer swaps while writing
	//{
	//	chibios_rt::MutexLocker lock(logBufferMutex);
//
	//	writeBuffer->writeLogger(logging);
	//}
//
	//// Reset the logging now that it's been written out
	logging->reset();
#endif
#endif
}
