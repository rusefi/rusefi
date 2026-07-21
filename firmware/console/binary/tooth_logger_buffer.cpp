/**
 * @file tooth_logger_buffer.cpp
 *
 * See tooth_logger_buffer.h - buffer lifecycle extracted from tooth_logger.cpp
 * so it can run in host unit tests.
 */

#include "pch.h"

#if EFI_TOOTH_LOGGER

#include "tooth_logger_buffer.h"

bool ToothLoggerBufferPool::startI() {
	m_bufferHandle = getBigBuffer(BigBufferUser::ToothLogger);
	if (!m_bufferHandle) {
		return false;
	}

	CompositeBuffer* buffers = m_bufferHandle.get<CompositeBuffer>();

	// Reset all buffers
	for (size_t i = 0; i < bufferCount; i++) {
		buffers[i].nextIdx = 0;
	}

	// Reset state
	m_currentBuffer = nullptr;

	m_freeBuffers.resumeX();
	m_filledBuffers.resumeX();

	// Put all buffers in the free list
	for (size_t i = 0; i < bufferCount; i++) {
		m_freeBuffers.postI(&buffers[i]);
	}

	return true;
}

void ToothLoggerBufferPool::stopI() {
	// Resume all waiting threads
	m_freeBuffers.resetI();
	m_filledBuffers.resetI();

	// Drop the partial buffer - it lives in memory we are about to hand back
	m_currentBuffer = nullptr;

	// Release the big buffer for another user
	// C++ magic: here we are calling BigBufferHandle::operator=() with empty instance
	m_bufferHandle = {};
}

CompositeBuffer* ToothLoggerBufferPool::findBufferI(efitick_t timestamp) {
	CompositeBuffer* buffer;

	if (!m_currentBuffer) {
		// try and find a buffer, if none available, we can't log
		if (MSG_OK != m_freeBuffers.fetchI(&buffer)) {
			return nullptr;
		}

		// Record the time of the last buffer swap so we can force a swap after a minimum period of time
		// This ensures the user sees *something* even if they don't have enough trigger events
		// to fill the buffer.
		buffer->startTime.reset(timestamp);
		buffer->nextIdx = 0;

		m_currentBuffer = buffer;
	}

	return m_currentBuffer;
}

void ToothLoggerBufferPool::appendI(const composite_logger_s& state, efitick_t timestamp) {
	CompositeBuffer* buffer = findBufferI(timestamp);

	if (!buffer) {
		// All buffers are full, nothing to do here.
		return;
	}

	size_t idx = buffer->nextIdx;
	auto nextIdx = idx + 1;
	buffer->nextIdx = nextIdx;

	if (idx < efi::size(buffer->buffer)) {
		composite_logger_s* entry = &buffer->buffer[idx];

		entry->x = state.x;
		// timestamp is offset to buffer begin
		entry->timestamp = NT2US(timestamp - buffer->startTime.get());

		// TS uses big endian, grumble
		// the whole order of all packet bytes is reversed, not just the 'endian-swap' integers
		// swap whole record byteorder
		entry->x = SWAP_UINT64(entry->x);
	}

	// if the buffer is full...
	bool bufferFull = nextIdx >= efi::size(buffer->buffer);
	// ... or it's been too long since the last flush
	bool bufferTimedOut = buffer->startTime.hasElapsedSec(5);

	// Then cycle buffers and set the ready flag.
	if (bufferFull || bufferTimedOut) {
		// Post to the output queue
		m_filledBuffers.postI(buffer);

		// Null the current buffer so we get a new one next time
		m_currentBuffer = nullptr;

		// Flag that we are ready
		setReady(true);
	}
}

void ToothLoggerBufferPool::returnBufferI(CompositeBuffer* buffer) {
	// ignore return, nothing we can do in case of error.
	// MSG_RESET is possible if tooth logger was disabled while buffer was outside
	m_freeBuffers.postI(buffer);

	// If the used list is empty, clear the ready flag
	if (m_filledBuffers.getUsedCountI() == 0) {
		setReady(false);
	}
}

CompositeBuffer* ToothLoggerBufferPool::getFilled(sysinterval_t timeout) {
	CompositeBuffer* buffer = nullptr;
	msg_t msg = m_filledBuffers.fetch(&buffer, timeout);

	if (msg == MSG_TIMEOUT) {
		setReady(false);
		return nullptr;
	}

	if (msg != MSG_OK) {
		// someone just disabled tooth logger and reset queues?
		// What even happened if we didn't get timeout, but also didn't get OK?
		return nullptr;
	}

	return buffer;
}

msg_t ToothLoggerBufferPool::fetchFilled(CompositeBuffer** buffer, sysinterval_t timeout) {
	return m_filledBuffers.fetch(buffer, timeout);
}

CompositeBuffer* ToothLoggerBufferPool::flushCurrentI() {
	CompositeBuffer* buffer = m_currentBuffer;
	m_currentBuffer = nullptr;
	return buffer;
}

bool ToothLoggerBufferPool::hasDataI() {
	return (m_currentBuffer) ||
		(m_filledBuffers.getUsedCountI() > 0);
}

#endif // EFI_TOOTH_LOGGER
