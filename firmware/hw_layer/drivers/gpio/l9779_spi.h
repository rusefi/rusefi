#pragma once

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
