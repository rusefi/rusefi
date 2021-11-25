#include "log_field.h"
#include "buffered_writer.h"

#include <cstring>

static void memcpy_swapend(void* dest, const void* src, size_t num) {
	const char* src2 = reinterpret_cast<const char*>(src);
	char* dest2 = reinterpret_cast<char*>(dest);
	
	for (size_t i = 0; i < num; i++) {
		// Endian swap - copy the end to the beginning
		dest2[i] = src2[num - 1 - i];
	}
}

static void copyFloat(char* buffer, float value) {
	memcpy_swapend(buffer, reinterpret_cast<char*>(&value), sizeof(float));
}

void LogField::writeHeader(Writer& outBuffer) const {
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

	// Total size = 55
	outBuffer.write(buffer, MLQ_FIELD_HEADER_SIZE);
}

size_t LogField::writeData(char* buffer) const {
	size_t size = m_size;

	memcpy_swapend(buffer, m_addr, size);

	return size;
}
