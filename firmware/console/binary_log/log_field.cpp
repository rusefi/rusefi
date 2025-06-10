#include "pch.h"
#include "log_field.h"
#include "buffered_writer.h"

#include <cstring>

static void memcpy_swapend(void* dest, const void* src, size_t num, void *offset) {
	const char* src2 = reinterpret_cast<const char*>(src);
	char* dest2 = reinterpret_cast<char*>(dest);
	for (size_t i = 0; i < num; i++) {
		// Endian swap - copy the end to the beginning
		dest2[i] = src2[num - 1 - i + (uint64_t)offset];
	}
}

static void copyFloat(char* buffer, float value) {
	memcpy_swapend(buffer, &value, sizeof(float), nullptr);
}

size_t LogField::writeHeader(Writer& outBuffer) const {
	char buffer[MLQ_FIELD_HEADER_SIZE];

	// Offset 0, length 1 = type
	buffer[0] = static_cast<char>(m_type);

	// Offset 1, length 34 = name
	strncpy(&buffer[1], m_name, 34);

	// Offset 35, length 10 = units
	strncpy(&buffer[35], m_units, 10);

	// Offset 45, length 1 = Display style
	// value 0 -> floating point number
	buffer[45] = 0;

	// Offset 46, length 4 = Scale
	copyFloat(buffer + 46, m_multiplier);

	// Offset 50, length 4 = shift before scaling (always 0)
	copyFloat(buffer + 50, 0);

	// Offset 54, size 1 = digits to display (signed int)
	buffer[54] = m_digits;

	// Offset 55, (optional) category string
	if (m_category) {
		size_t categoryLength = strlen(m_category);
		size_t lengthAfterCategory = 34 - categoryLength;
		memcpy(&buffer[55], m_category, categoryLength);
		memset(&buffer[55] + categoryLength, 0, lengthAfterCategory);
	} else {
		memset(&buffer[55], 0, 34);
	}

	// Total size = 89
	outBuffer.write(buffer, MLQ_FIELD_HEADER_SIZE);

	return MLQ_FIELD_HEADER_SIZE;
}

size_t LogField::writeData(char* buffer, void *offset) const {
    if (m_isBitField) {
        const char* const bitsBlockAddr = static_cast<const char*>(m_addr) + m_bitsBlockOffset;
        const char* const byteWithBitAddr = bitsBlockAddr + m_bitNumber / 8;
        unsigned char byteWithBit = 0;
        memcpy_swapend(&byteWithBit, byteWithBitAddr, 1, offset);
        const uint8_t bitNumberInByte = m_bitNumber % 8;
        buffer[0] = static_cast<char>(static_cast<bool>(byteWithBit & (1 << bitNumberInByte)));
    } else {
        memcpy_swapend(buffer, m_addr, m_size, offset);
    }

	return m_size;
}
