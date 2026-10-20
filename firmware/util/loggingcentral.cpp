/**
 * @file	loggingcentral.cpp
 *
 * This file implements text logging.
 *
 * Uses a queue of buffers so that the expensive printf operation doesn't require exclusive access
 * (ie, global system lock) to log.  In the past there have been serious performance problems caused
 * by heavy logging on a low priority thread that blocks the rest of the system running (trigger errors, etc).
 *
 * Uses ChibiOS mailboxes to maintain one queue of free line buffers, and one queue of filled buffers.
 * When a thread wants to write, it acquires a free line buffer, prints to it, and posts it in to the
 * filled queue. There is no dedicated flusher thread and no intermediate large output buffer: the
 * binary TS thread itself drains the filled queue in loggingSendOutputData() and sends several
 * buffers back to back as one TS packet, releasing each buffer via LogBufferBase::free() right
 * after it has been written out.
 *
 * Buffers are not limited to the internal fixed-size line buffers: any LogBufferBase owner (see the
 * engine sniffer) may fill its own buffer and hand it over with loggingPostBuffer(). Such a buffer is
 * returned to its owner by its own free() implementation once transferred. This saves RAM since the
 * data is never copied into a second buffer.
 *
 * @date Mar 8, 2015, heavily revised April 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 * @author Matthew Kennedy
 */

#include "pch.h"

/* for isprint() */
#include <ctype.h>

#if EFI_UNIT_TEST
#include "mock-mailbox.h"
#endif

#if EFI_TEXT_LOGGING

// Stores the result of one call to efiPrintfInternal while it waits in the queue to be sent to TS.
// Returns itself to the free queue once it has been transferred
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


// actually we can send much more in one packed. this is just a threshold that triggers current packet finalization
constexpr size_t maxSend = scratchBuffer_SIZE;

/**
 * Write as much data as possible
 *
 * @return actual size transfered
 */

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

// Despite the historical name there is no logging thread to start anymore:
// this just makes all line buffers available to the writers
void startLoggingProcessor() {
	// Push all buffers in to the free queue
	for (size_t i = 0; i < lineBufferCount; i++) {
		LogLineBuffer* line = &lineBuffers[i];

		line->len = 0;
		line->free();
	}
}

#if EFI_UNIT_TEST
void resetLoggingForUnitTest() {
	// drop whatever previous tests left queued, without calling free() on
	// buffers whose owners may already be gone
	filledBuffers.resetI();
	filledBuffers.resumeX();
	freeBuffers.resetI();
	freeBuffers.resumeX();
	startLoggingProcessor();
}
#endif // EFI_UNIT_TEST

#endif // EFI_TEXT_LOGGING

#if EFI_UNIT_TEST || EFI_SIMULATOR
extern bool verboseMode;
#endif

size_t maxPrintfSize = 0;

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
#if EFI_TEXT_LOGGING
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

	// Write the formatted string to the line buffer
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
	lineBuffer->len = len;

	if (len > maxPrintfSize) {
		maxPrintfSize = len;
	}

	loggingPostBuffer(lineBuffer);
#endif
}
} // namespace priv

#if EFI_TEXT_LOGGING
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
#else
void loggingPostBuffer(LogBufferBase *) {
}
#endif // EFI_TEXT_LOGGING
