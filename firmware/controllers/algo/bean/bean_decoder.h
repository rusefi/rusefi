#pragma once

#include <cstdint>
#include "rusefi/containers/cyclic_buffer.h"

// Message buffer size: longest BEAN message is ~12 bytes
static const int BEAN_MESSAGE_MAX_SIZE = 16;
// Rolling buffer of all decoded bytes for diagnostics/testing
static const int BEAN_ALL_BYTES_BUFFER_SIZE = 128;

/**
 * Toyota BEAN (Body Electronics Area Network) protocol decoder.
 *
 * BEAN is a single-wire bus running at 10 kbit/s (100us per bit).
 * The protocol uses bit-stuffing (after 5 consecutive same-value bits,
 * a complementary stuff bit is inserted) and marks end-of-message with
 * specific byte values.
 *
 * This decoder processes individual bits from a logic analyzer capture,
 * handles de-stuffing, assembles bytes, and detects message boundaries.
 */
class BeanDecoder {
public:
	/**
	 * Feed a CSV edge into the decoder. Computes how many bit periods
	 * elapsed since the last edge, then calls processBit() for each one.
	 *
	 * @param timestamp  absolute time in seconds of this edge
	 * @param value      logic level (true = high) at this edge
	 */
	void processEdge(double timestamp, bool value) {
		if (!m_hasFirstEdge) {
			m_lastTimestamp = timestamp;
			m_lastValue = value;
			m_hasFirstEdge = true;
			return;
		}

		double duration = timestamp - m_lastTimestamp;
		// 100us per bit, with small offset (30us) to compensate for rounding
		// in logic analyzer sampling
		int bitCount = (int)((duration + 0.00003) / 0.0001);
		// Cap unreasonably long gaps (idle bus) to a single bit
		if (bitCount > 50) bitCount = 1;

		for (int i = 0; i < bitCount; i++) {
			processBit(m_lastValue);
		}

		m_lastTimestamp = timestamp;
		m_lastValue = value;
	}

	/**
	 * Rolling buffer of all decoded bytes, including EOM markers and any noise/preamble.
	 * Used by tests to verify the decoded output. Being a cyclic buffer of fixed size,
	 * only the most recent BEAN_ALL_BYTES_BUFFER_SIZE bytes are retained.
	 */
	cyclic_buffer<uint8_t, BEAN_ALL_BYTES_BUFFER_SIZE> m_allBytes;

	/**
	 * Total number of bytes decoded since reset (including those that have
	 * rolled out of the cyclic buffer).
	 */
	size_t m_totalBytesDecoded = 0;

private:
	/**
	 * Process a single decoded bit through the BEAN protocol state machine.
	 *
	 * Handles bit-stuffing removal: after 5 consecutive bits of the same
	 * polarity, the next bit is a stuff bit inserted by the transmitter
	 * and must be discarded (not shifted into the byte register).
	 *
	 * Once 8 data bits are accumulated, the resulting byte is stored and
	 * checked for end-of-message markers.
	 */
	void processBit(bool bit) {
		if (m_stuffingCount == 5) {
			// After 5 consecutive same-value bits, this bit is a stuff bit
			// inserted by the transmitter for clock recovery — discard it
			m_stuffingCount = 1;
			m_lastBit = bit;
			return;
		}

		if (bit == m_lastBit) {
			m_stuffingCount++;
		} else {
			m_stuffingCount = 1;
			m_lastBit = bit;
		}

		m_currentByte = (m_currentByte << 1) | (bit ? 1 : 0);
		m_bitCount++;

		if (m_bitCount == 8) {
			m_allBytes.add(m_currentByte);
			m_totalBytesDecoded++;
			m_message.add(m_currentByte);
			/**
			 * 0x7E is the canonical BEAN End-of-Message (EOM) marker.
			 * 0x7C and 0x7D are also accepted because logic analyzer
			 * bit-sampling jitter can cause the last 1-2 bits of the
			 * EOM byte to be misread (off-by-one in the LSBs).
			 * All three share the upper 6 bits 0b011111xx.
			 */
			if (m_currentByte == 0x7E || m_currentByte == 0x7C || m_currentByte == 0x7D) {
				// End of message boundary detected
				m_message.clear();
			}
			m_bitCount = 0;
			m_currentByte = 0;
		}
	}

	// Bit assembly state
	uint8_t m_currentByte = 0;
	int m_bitCount = 0;

	// Bit-stuffing tracking
	int m_stuffingCount = 0;
	bool m_lastBit = false;

	// Current message accumulator (reset on EOM)
	cyclic_buffer<uint8_t, BEAN_MESSAGE_MAX_SIZE> m_message;

	// Edge timing state
	double m_lastTimestamp = 0;
	bool m_lastValue = false;
	bool m_hasFirstEdge = false;
};
