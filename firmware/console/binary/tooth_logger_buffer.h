/**
 * @file tooth_logger_buffer.h
 *
 * Multi-buffering between tooth-event producers (trigger/coil/injector edge
 * handlers, interrupt context) and consumers (TunerStudio composite reader,
 * SD card .teeth writer). Extracted from tooth_logger.cpp so the buffer
 * lifecycle is also compiled - and testable - in host unit tests, where it
 * runs against the mailbox mock in unit_tests/chibios-mock/mock-mailbox.h.
 *
 * Buffers live in the shared BigBuffer region, so the tooth logger cannot run
 * concurrently with other BigBuffer users. See docs/AI/sd_card_logging.md
 */

#pragma once

#include "tooth_logger.h"
#include "big_buffer.h"

#if EFI_UNIT_TEST
#include "mock-mailbox.h"
#endif

#if EFI_TOOTH_LOGGER

/* How many CompositeBuffer will be STATICALY allocated for circle logging
 * non zero value also enables circular buffer mode */
#ifndef EFI_TOOTH_LOGGER_STATICBUFFER_COUNT
#if EFI_UNIT_TEST
// host unit tests exercise the BigBuffer-backed, non-circular path
#define EFI_TOOTH_LOGGER_STATICBUFFER_COUNT 0
#else
#define EFI_TOOTH_LOGGER_STATICBUFFER_COUNT 4
#endif
#endif

#if (EFI_TOOTH_LOGGER_STATICBUFFER_COUNT == 1)
	#error "EFI_TOOTH_LOGGER_STATICBUFFER_COUNT should be at least 2"
#endif

class ToothLoggerBufferPool {
public:
	// Consumer-visible "a filled buffer is ready" indication
	// (drives outputChannels.toothLogReady in production)
	using ReadyCallback = void(*)(bool ready);

	ToothLoggerBufferPool() = default;
	explicit ToothLoggerBufferPool(ReadyCallback callback)
		: m_onReady(callback)
	{
	}

	// Acquire the big buffer and place every buffer on the (re-armed) free
	// list. Returns false if the big buffer is taken by another user.
	// Caller must hold the critical section.
	bool startI();

	// Empty both queues - waking any waiting consumer with MSG_RESET - and
	// release the big buffer. Caller must hold the critical section.
	void stopI();

	// Append one entry carrying the given flag state; the entry's timestamp is
	// stored as a microsecond offset from the owning buffer's startTime. Posts
	// the buffer to the filled queue when it fills up or goes 5 seconds stale.
	// Caller must hold the critical section.
	void appendI(const composite_logger_s& state, efitick_t timestamp);

	// Return a drained buffer to the free list.
	// Caller must hold the critical section.
	void returnBufferI(CompositeBuffer* buffer);

	// Fetch a filled buffer, maintaining the ready indication.
	// Returns nullptr if none is available.
	// Thread context, no critical section held.
	CompositeBuffer* getFilled(sysinterval_t timeout);

	// Raw fetch with no ready-indication side effect (SD writer path).
	// Thread context, no critical section held.
	msg_t fetchFilled(CompositeBuffer** buffer, sysinterval_t timeout);

	// Take the partially-filled current buffer away (SD idle flush).
	// Returns nullptr if there is no current buffer.
	// Caller must hold the critical section.
	CompositeBuffer* flushCurrentI();

	// Post the partially-filled current buffer to the filled queue.
	// Returns true if there was a current buffer to post.
	// Caller must hold the critical section.
	bool postCurrentI();

	// Peek at the partially-filled current buffer, may return nullptr.
	CompositeBuffer* getCurrent() const {
		return m_currentBuffer;
	}

	// In circular mode the oldest filled buffer is overwritten when no
	// consumer drains the filled queue, and stale buffers are not flushed.
	bool isCircularMode() const {
		return m_circularMode;
	}

	void setCircularModeI(bool circular);

	// True if any entries are pending: a partial current buffer or a filled
	// buffer waiting for a consumer. Caller must hold the critical section.
	bool hasDataI();

#if (EFI_TOOTH_LOGGER_STATICBUFFER_COUNT > 0)
	static constexpr size_t bufferCount = EFI_TOOTH_LOGGER_STATICBUFFER_COUNT;
#else
	static constexpr size_t bufferCount = BIG_BUFFER_SIZE / sizeof(CompositeBuffer);
#endif
	static constexpr size_t toothLoggerEntriesTotal = toothLoggerEntriesPerBuffer * bufferCount;

private:
	CompositeBuffer* findBufferI(efitick_t timestamp);

	void setReady(bool ready) {
		if (m_onReady) {
			m_onReady(ready);
		}
	}

	chibios_rt::Mailbox<CompositeBuffer*, bufferCount> m_freeBuffers;
	chibios_rt::Mailbox<CompositeBuffer*, bufferCount> m_filledBuffers;

	CompositeBuffer* m_currentBuffer = nullptr;
	// do not update currentBuffer->startTime until the buffer is posted, as in
	// circular mode a reused buffer still contains old entries
	Timer m_currentBufferStartTime = {};

#if (EFI_TOOTH_LOGGER_STATICBUFFER_COUNT > 0)
	CompositeBuffer m_buffers[EFI_TOOTH_LOGGER_STATICBUFFER_COUNT];
#else
	BigBufferHandle m_bufferHandle;
#endif
	// rewrite older events with newer if noone is reading filledBuffers queue
	bool m_circularMode = false;

	ReadyCallback m_onReady = nullptr;
};

static_assert(ToothLoggerBufferPool::bufferCount >= 2);

#endif // EFI_TOOTH_LOGGER
