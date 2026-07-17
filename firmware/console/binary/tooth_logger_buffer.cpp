/**
 * @file tooth_logger_buffer.cpp
 *
 * See tooth_logger_buffer.h - buffer lifecycle extracted from tooth_logger.cpp
 * so it can run in host unit tests.
 */

#include "pch.h"

#if EFI_TOOTH_LOGGER

#include "tooth_logger_buffer.h"
#include "board_overrides.h"

// Defined here (not tooth_logger.cpp) because appendI needs it in every
// EFI_TOOTH_LOGGER build, including TS-composite-only boards without
// EFI_FILE_LOGGING.
std::optional<board_tooth_log_sample_type> custom_board_toothLogSample;

bool ToothLoggerBufferPool::startI() {
#if (EFI_TOOTH_LOGGER_STATICBUFFER_COUNT > 0)
	// we already have static buffers
	CompositeBuffer* buffers = m_buffers;
#else
	m_bufferHandle = getBigBuffer(BigBufferUser::ToothLogger);
	if (!m_bufferHandle) {
		return false;
	}

	CompositeBuffer* buffers = m_bufferHandle.get<CompositeBuffer>();
#endif

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

#if (EFI_TOOTH_LOGGER_STATICBUFFER_COUNT > 0)
	// static buffers, nothing to release
#else
	// Release the big buffer for another user
	// C++ magic: here we are calling BigBufferHandle::operator=() with empty instance
	m_bufferHandle = {};
#endif
}

CompositeBuffer* ToothLoggerBufferPool::findBufferI(efitick_t timestamp) {
	CompositeBuffer* buffer;

	if (!m_currentBuffer) {
		// try and find a buffer, if none available, we can't log
		if (MSG_OK != m_freeBuffers.fetchI(&buffer)) {
			if (!m_circularMode) {
				return nullptr;
			}

			// in circular mode get oldest one and reuse
			if (MSG_OK != m_filledBuffers.fetchI(&buffer)) {
				return nullptr;
			}
		}

		// Record the time of the last buffer swap so we can force a swap after a minimum period of time
		// This ensures the user sees *something* even if they don't have enough trigger events
		// to fill the buffer.
		m_currentBufferStartTime.reset(timestamp);
		buffer->nextIdx = 0;

		m_currentBuffer = buffer;
	}

	return m_currentBuffer;
}

void ToothLoggerBufferPool::appendI(const composite_logger_s& state, efitick_t timestamp) {
	if (!m_circularMode) {
		// check for timeout only in normal mode
		if ((m_currentBuffer) && (m_currentBuffer->nextIdx > 0) &&
			(m_currentBufferStartTime.hasElapsedSec(5))) {
			// more than 5 seconds gap in events - start new buffer

			postCurrentI();

			// Flag that we are ready
			setReady(true);
		}
	}

	CompositeBuffer* buffer = findBufferI(timestamp);
	if (!buffer) {
		// All buffers are full, nothing to do here.
		return;
	}

	// TODO: why so complicated?
	size_t idx = buffer->nextIdx;
	auto nextIdx = idx + 1;
	buffer->nextIdx = nextIdx;

	// TODO: is this a useless check?
	if (idx < efi::size(buffer->buffer)) {
		composite_logger_s* entry = &buffer->buffer[idx];

		entry->x = state.x;
		// timestamp is offset to buffer begin
		entry->timestamp = NT2US(timestamp - m_currentBufferStartTime.get());

		// TS uses big endian, grumble
		// the whole order of all packet bytes is reversed, not just the 'endian-swap' integers
		// swap whole record byteorder
		entry->x = SWAP_UINT64(entry->x);

#if EFI_FILE_LOGGING || EFI_UNIT_TEST
		// Values behind the VBatt/ET/InstantMAP/TPS .teeth CSV columns, sampled
		// NOW (event time) so they are not skewed by buffer residency.
		composite_sensor_snapshot_s& snapshot = buffer->sensorSnapshot[idx];
		snapshot.vbatt = Sensor::get(SensorType::BatteryVoltage).value_or(0);
		snapshot.et = Sensor::get(SensorType::Clt).value_or(0);
		snapshot.instantMap = (float)engine->outputChannels.instantMAPValue;
		snapshot.tps = Sensor::get(SensorType::Tps1).value_or(0);
#endif

#if TOOTH_LOG_BOARD_PAYLOAD_SIZE > 0
		// Board per-event payload, sampled NOW (event time) so the .teeth CSV
		// columns it feeds are not skewed by buffer residency the way the
		// flush-time-sampled upstream columns are.
		if (custom_board_toothLogSample.has_value()) {
			(*custom_board_toothLogSample)(buffer->boardPayload[idx]);
		} else {
			memset(buffer->boardPayload[idx], 0, TOOTH_LOG_BOARD_PAYLOAD_SIZE);
		}
#endif
	}

	// Then cycle buffers and set the ready flag.
	if (nextIdx >= efi::size(buffer->buffer)) {
		// Post to the output queue
		postCurrentI();

		if (!m_circularMode) {
			// Flag that we are ready
			setReady(true);
		}
	}
}

bool ToothLoggerBufferPool::postCurrentI() {
	if (!m_currentBuffer) {
		return false;
	}

	// startTime is deferred until the buffer is posted - see m_currentBufferStartTime
	m_currentBuffer->startTime = m_currentBufferStartTime;

	// Post to the output queue
	m_filledBuffers.postI(m_currentBuffer);

	// Null the current buffer so we get a new one next time
	m_currentBuffer = nullptr;

	return true;
}

void ToothLoggerBufferPool::returnBufferI(CompositeBuffer* buffer) {
	// ignore return, nothing we can do in case of error.
	// MSG_RESET is possible if tooth logger was disabled while buffer was outside
	m_freeBuffers.postI(buffer);

	// If the used list is empty, clear the ready flag
	if (m_filledBuffers.getUsedCountI() == 0) {
		if (!m_circularMode) {
			setReady(false);
		}
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
	if (buffer) {
		// startTime is deferred until the buffer leaves the pool
		buffer->startTime = m_currentBufferStartTime;
	}
	m_currentBuffer = nullptr;
	return buffer;
}

void ToothLoggerBufferPool::setCircularModeI(bool circular) {
	m_circularMode = circular;
}

bool ToothLoggerBufferPool::hasDataI() {
	return (m_currentBuffer) ||
		(m_filledBuffers.getUsedCountI() > 0);
}

#endif // EFI_TOOTH_LOGGER
