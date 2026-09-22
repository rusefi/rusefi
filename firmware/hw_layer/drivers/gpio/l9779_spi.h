#pragma once

#include <cstddef>
#include <cstdint>

inline bool l9779HasOddParity(uint16_t word) {
	word ^= word >> 8;
	word ^= word >> 4;
	word ^= word >> 2;
	word ^= word >> 1;
	return (word & 1) != 0;
}

// Bit 0 makes the complete L9779 SPI frame odd parity.
inline uint16_t l9779PrepareSpiWord(uint16_t word) {
	word &= static_cast<uint16_t>(~1U);
	return word | !l9779HasOddParity(word);
}

constexpr uint32_t L9779_DIRECT_DRIVE_MASK = 0x7ffU;

struct L9779OutputRegisters {
	uint32_t enabledState;
	uint8_t control[4];
};

inline L9779OutputRegisters l9779PackOutputRegisters(uint32_t outputState, uint32_t outputEnableMask) {
	/* IGN1..4 and OUT1..7 are enabled through SPI, then switched by their
	 * dedicated parallel inputs. Keep those enables independent of the live
	 * output state; all other channels are controlled directly over SPI. */
	const uint32_t enabledState =
		(outputState & ~L9779_DIRECT_DRIVE_MASK) |
		(outputEnableMask & L9779_DIRECT_DRIVE_MASK);

	const auto outputBit = [enabledState](unsigned int output, unsigned int bit) -> uint8_t {
		return static_cast<uint8_t>(((enabledState >> (output + 3U)) & 1U) << bit);
	};
	const auto ignitionBit = [enabledState](unsigned int ignition, unsigned int bit) -> uint8_t {
		return static_cast<uint8_t>(((enabledState >> (ignition - 1U)) & 1U) << bit);
	};

	L9779OutputRegisters result = { enabledState, {} };
	result.control[0] =
		outputBit( 1, 7) |
		outputBit( 2, 6) |
		outputBit( 3, 5) |
		outputBit( 4, 4) |
		outputBit( 5, 3) |
		outputBit(20, 2);
	result.control[1] =
		outputBit(15, 7) |
		outputBit(14, 6) |
		ignitionBit(1, 3) |
		ignitionBit(2, 2) |
		ignitionBit(3, 1) |
		ignitionBit(4, 0);
	result.control[2] =
		outputBit(22, 7) |
		outputBit(21, 6) |
		outputBit(16, 5) |
		outputBit(13, 4) |
		outputBit(17, 3) |
		outputBit(18, 2) |
		outputBit( 7, 1) |
		outputBit( 6, 0);
	result.control[3] =
		outputBit(28, 5) |
		outputBit(27, 4) |
		outputBit(26, 3) |
		outputBit(25, 2) |
		outputBit(24, 1) |
		outputBit(23, 0);

	return result;
}

class L9779ReadTracker {
public:
	static constexpr size_t Capacity = 8;

	bool push(uint8_t subaddress) {
		if (m_count == Capacity) {
			return false;
		}

		m_pending[m_count++] = subaddress;
		return true;
	}

	bool consume(uint8_t subaddress) {
		for (size_t i = 0; i < m_count; i++) {
			if (m_pending[i] != subaddress) {
				continue;
			}

			for (size_t j = i + 1; j < m_count; j++) {
				m_pending[j - 1] = m_pending[j];
			}

			m_count--;
			return true;
		}

		return false;
	}

	void clear() {
		m_count = 0;
	}

	size_t size() const {
		return m_count;
	}

private:
	uint8_t m_pending[Capacity] = {};
	size_t m_count = 0;
};

struct L9779SpiFrame {
	uint16_t tx;
	uint16_t rx;
	uint8_t subaddress;
	int8_t result;
};

class L9779SpiFrameLog {
public:
	static constexpr size_t Capacity = 32;

	void record(uint16_t tx, uint16_t rx, uint8_t subaddress, int result) {
		m_frames[m_next] = { tx, rx, subaddress, static_cast<int8_t>(result) };
		m_next = (m_next + 1) % Capacity;
		if (m_count < Capacity) {
			m_count++;
		}
	}

	const L9779SpiFrame* get(size_t chronologicalIndex) const {
		if (chronologicalIndex >= m_count) {
			return nullptr;
		}

		const size_t oldest = (m_next + Capacity - m_count) % Capacity;
		return &m_frames[(oldest + chronologicalIndex) % Capacity];
	}

	size_t size() const {
		return m_count;
	}

private:
	L9779SpiFrame m_frames[Capacity] = {};
	size_t m_count = 0;
	size_t m_next = 0;
};
