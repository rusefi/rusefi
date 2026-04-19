#pragma once

#include <cstdint>

/**
 * Toyota BEAN (Body Electronics Area Network) protocol encoder.
 *
 * Generates a sequence of timed edges (timestamp + logic level) representing
 * the physical layer encoding of a BEAN message. Handles bit-stuffing
 * (inserts a complementary bit after 5 consecutive same-value bits) and
 * uses 100us per bit timing (10 kbit/s).
 */
class BeanEncoder {
public:
	// 100us per bit at 10 kbit/s
	static constexpr double BIT_PERIOD = 0.0001;

	/**
	 * Reset encoder state and set the starting timestamp.
	 */
	void reset(double startTimestamp = 0.0) {
		m_timestamp = startTimestamp;
		m_currentLevel = false;
		m_stuffingCount = 0;
		m_lastBit = false;
		m_edgeCount = 0;
	}

	/**
	 * Encode a complete byte into the bit stream, applying bit-stuffing.
	 * After this call, getNextTimestamp() returns the time after the last
	 * bit of this byte (plus any stuff bits).
	 */
	void encodeByte(uint8_t byte) {
		for (int i = 7; i >= 0; i--) {
			bool bit = (byte >> i) & 1;
			encodeBit(bit);
		}
	}

	/**
	 * Encode an array of bytes.
	 */
	void encodeBytes(const uint8_t* data, int length) {
		for (int i = 0; i < length; i++) {
			encodeByte(data[i]);
		}
	}

	/**
	 * Return the current timestamp (time of the next edge or end of last bit).
	 */
	double getNextTimestamp() const {
		return m_timestamp;
	}

	/**
	 * Total number of edges generated so far.
	 */
	int getEdgeCount() const {
		return m_edgeCount;
	}

	/**
	 * Return the current logic level of the encoded signal.
	 */
	bool getCurrentLevel() const {
		return m_currentLevel;
	}

	/**
	 * Callback type for edge output: (timestamp, logicLevel).
	 */
	typedef void (*EdgeCallback)(double timestamp, bool level, void* userData);

	/**
	 * Set a callback to receive each generated edge.
	 */
	void setEdgeCallback(EdgeCallback cb, void* userData) {
		m_callback = cb;
		m_userData = userData;
	}

private:
	/**
	 * Encode a single data bit, inserting stuff bits as needed.
	 * After 5 consecutive same-value bits, a complementary stuff bit
	 * is automatically inserted before the next data bit.
	 */
	void encodeBit(bool bit) {
		// Check if we need to insert a stuff bit first
		if (m_stuffingCount == 5) {
			// Insert complementary stuff bit
			emitBit(!m_lastBit);
			m_stuffingCount = 1;
			m_lastBit = !m_lastBit;
		}

		// Emit the actual data bit
		emitBit(bit);

		if (bit == m_lastBit) {
			m_stuffingCount++;
		} else {
			m_stuffingCount = 1;
			m_lastBit = bit;
		}
	}

	/**
	 * Emit a single bit on the wire: if the level changes, generate an
	 * edge at the current timestamp; then advance time by one bit period.
	 */
	void emitBit(bool bit) {
		if (bit != m_currentLevel) {
			if (m_callback) {
				m_callback(m_timestamp, bit, m_userData);
			}
			m_currentLevel = bit;
			m_edgeCount++;
		}
		m_timestamp += BIT_PERIOD;
	}

	double m_timestamp = 0;
	bool m_currentLevel = false;
	int m_stuffingCount = 0;
	bool m_lastBit = false;
	int m_edgeCount = 0;

	EdgeCallback m_callback = nullptr;
	void* m_userData = nullptr;
};
