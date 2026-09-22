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

struct L9779VrsConfiguration {
	uint8_t config1;
	uint8_t config5;
};

inline constexpr L9779VrsConfiguration l9779FullAdaptiveVrsConfiguration() {
	return {
		0x02, // Full-adaptive VRS mode.
		0xd8, // Adaptive hysteresis and time filter, 17 uA floor.
	};
}

inline constexpr uint8_t L9779_WDA_RESPONSES[16][4] = {
	{0xff, 0x0f, 0xf0, 0x00},
	{0xb0, 0x40, 0xbf, 0x4f},
	{0xe9, 0x19, 0xe6, 0x16},
	{0xa6, 0x56, 0xa9, 0x59},
	{0x75, 0x85, 0x7a, 0x8a},
	{0x3a, 0xca, 0x35, 0xc5},
	{0x63, 0x93, 0x6c, 0x9c},
	{0x2c, 0xdc, 0x23, 0xd3},
	{0xd2, 0x22, 0xdd, 0x2d},
	{0x9d, 0x6d, 0x92, 0x62},
	{0xc4, 0x34, 0xcb, 0x3b},
	{0x8b, 0x7b, 0x84, 0x74},
	{0x58, 0xa8, 0x57, 0xa7},
	{0x17, 0xe7, 0x18, 0xe8},
	{0x4e, 0xbe, 0x41, 0xb1},
	{0x01, 0xf1, 0x0e, 0xfe},
};

struct L9779WdaStatus {
	uint8_t question;
	uint8_t errorCount;
	bool interrupt;
};

inline constexpr L9779WdaStatus l9779DecodeWdaStatus(uint8_t requlo) {
	return {
		static_cast<uint8_t>(requlo & 0x0fU),
		static_cast<uint8_t>((requlo >> 4) & 0x07U),
		(requlo & 0x80U) != 0,
	};
}

inline constexpr bool l9779WdaResponseCounterAligned(uint8_t requhi) {
	return (requhi & 0xc0U) == 0xc0U;
}

inline constexpr int l9779AdjustWdaDelay(int delayMs, uint8_t requhi) {
	// NO_RESP takes precedence when late also appears early to the next cycle.
	if ((requhi & 0x02U) != 0) {
		delayMs -= 5;
	} else if ((requhi & 0x01U) != 0) {
		delayMs += 5;
	}

	return delayMs < 17 ? 17 : (delayMs > 38 ? 38 : delayMs);
}

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

enum class L9779DiagResult : uint8_t {
	ShortToGround = 0,
	OpenLoad = 1,
	ShortToBattery = 2,
	Ok = 3,
};

struct L9779DiagLocation {
	int8_t registerIndex;
	uint8_t shift;

	bool supported() const {
		return registerIndex >= 0;
	}
};

inline L9779DiagLocation l9779GetDiagLocation(size_t pin) {
	if (pin < 4) {
		return { 7, static_cast<uint8_t>(pin * 2) };
	}

	if (pin < 11) {
		const size_t output = pin - 4;
		return {
			static_cast<int8_t>(output / 4),
			static_cast<uint8_t>((output % 4) * 2),
		};
	}

	if (pin >= 16 && pin < 18) {
		return { 2, static_cast<uint8_t>(4 + (pin - 16) * 2) };
	}

	if (pin >= 18 && pin < 22) {
		return { 3, static_cast<uint8_t>((pin - 18) * 2) };
	}

	if (pin == 23) {
		return { 4, 2 };
	}

	if (pin >= 24 && pin < 32) {
		return {
			static_cast<int8_t>(5 + (pin - 24) / 4),
			static_cast<uint8_t>(((pin - 24) % 4) * 2),
		};
	}

	return { -1, 0 };
}

inline L9779DiagResult l9779DecodeDiagField(uint8_t field) {
	return static_cast<L9779DiagResult>(field & 0x03U);
}

constexpr uint8_t L9779_DIA10_OV_RST = 0x01;
constexpr uint8_t L9779_DIA10_OUT_DIS = 0x02;
constexpr uint8_t L9779_DIA10_V3V3_UV = 0x04;
constexpr uint8_t L9779_DIA10_VDD5_OV = 0x08;
constexpr uint8_t L9779_DIA10_F2 = 0x10;
constexpr uint8_t L9779_DIA10_CRK_RST = 0x20;
constexpr uint8_t L9779_DIA10_F1 = 0x40;
constexpr uint8_t L9779_DIA10_TNL_RST = 0x80;

inline bool l9779Dia10HasOutDis(uint8_t value) {
	return (value & L9779_DIA10_OUT_DIS) != 0;
}

inline bool l9779Dia10LostConfiguration(uint8_t value) {
	constexpr uint8_t ResetEvents =
		L9779_DIA10_TNL_RST | L9779_DIA10_CRK_RST | L9779_DIA10_OV_RST;
	return (value & ResetEvents) != 0;
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
