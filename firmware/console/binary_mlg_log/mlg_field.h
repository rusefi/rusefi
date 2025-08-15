#pragma once

#include <cstdint>
#include <cstring>

#include <rusefi/scaled_channel.h>

#include "writer.h"
#include "mlg_types.h"

// For unit tests we are manipulating with storage in runtime so consteval is not possible.
// In prod builds we have engine and configs as global instances so all addresses to read data from
// must be known compile-time. If consteval fails then some runtime logic made its way into LogField and that moves
// LogField instance into RAM which is correct from code perspective but incorrect intent-wise and consume code and RAM
// for no real reason.
#if defined(EFI_UNIT_TEST) && EFI_UNIT_TEST
#define LOG_FIELD_CONSTNESS_SPECIFIER_METHODS constexpr
#define LOG_FIELD_CONSTNESS_SPECIFIER_STORAGE const
#else
#define LOG_FIELD_CONSTNESS_SPECIFIER_METHODS consteval
#define LOG_FIELD_CONSTNESS_SPECIFIER_STORAGE const constinit
#endif

namespace MLG::Entries {
using namespace MLG;

class Field {
public:
	// Scaled channels, memcpys data directly and describes format in header
	template <typename TValue, int TMult, int TDiv>
	LOG_FIELD_CONSTNESS_SPECIFIER_METHODS Field(const scaled_channel<TValue, TMult, TDiv>& toRead,
			   const char* name, const char* units, int8_t digits, const char* category = "none")
		: m_multiplier(float(TDiv) / TMult)
		, m_addr(toRead.getFirstByteAddr())
		, m_type_id(static_cast<std::underlying_type_t<Types::Field::Scalar>>(Types::Field::resolveBuiltInNumberType<TValue>()))
		, m_digits(digits)
		, m_size(Types::Field::sizeForType<Types::Field::resolveBuiltInNumberType<TValue>()>())
		, m_name(name)
		, m_units(units)
		, m_category(category)
		, m_isBitField(false)
		, m_bitsBlockOffset(0)
		, m_bitNumber(0)
	{
	}

	// Non-scaled channel, works for plain arithmetic types (int, float, uint8_t, etc)
	template <typename TValue, typename = typename std::enable_if<std::is_arithmetic_v<TValue>>::type>
	LOG_FIELD_CONSTNESS_SPECIFIER_METHODS Field(TValue& toRead,
			   const char* name, const char* units, int8_t digits, const char* category = "none")
		: m_multiplier(1)
		, m_addr(&toRead)
		, m_type_id(static_cast<std::underlying_type_t<Types::Field::Scalar>>(Types::Field::resolveBuiltInNumberType<TValue>()))
		, m_digits(digits)
		, m_size(Types::Field::sizeForType<Types::Field::resolveBuiltInNumberType<TValue>()>())
		, m_name(name)
		, m_units(units)
		, m_category(category)
		, m_isBitField(false)
		, m_bitsBlockOffset(0)
		, m_bitNumber(0)
	{
	}

	// Bit channel
	template <typename TValue>
	LOG_FIELD_CONSTNESS_SPECIFIER_METHODS Field(
		TValue& toRead,
		const uint32_t bitsBlockOffset,
		const uint8_t bitNumber,
		const char* name,
		const char* units,
		const char* category = "none"
	): m_multiplier(1)
		, m_addr(&toRead)
		, m_type_id(static_cast<std::underlying_type_t<Types::Field::Scalar>>(Types::Field::Scalar::U08))
		, m_digits(0)
		, m_size(1)
		, m_name(name)
		, m_units(units)
		, m_category(category)
		, m_isBitField(true)
		, m_bitsBlockOffset(bitsBlockOffset)
		, m_bitNumber(bitNumber)
    {
    }

	constexpr size_t getSize() const { return m_size; }
	constexpr const void* getAddr() const { return m_addr; }

	// Write the header data describing this field.
	// Returns the number of bytes written.
	size_t writeHeader(Writer& outBuffer) const {
		char buffer[Types::Field::DescriptorSize];

		// Offset 0, length 1 = type
		buffer[0] = static_cast<char>(m_type_id);

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
		outBuffer.write(buffer, Types::Field::DescriptorSize);

		return Types::Field::DescriptorSize;
	}

	// Write the field's data to the buffer.
	// Returns the number of bytes written.
	size_t writeData(char* buffer, void *offset) const {
		if (m_isBitField) {
			const char* const bitsBlockAddr = static_cast<const char*>(m_addr) + m_bitsBlockOffset;
			const char* const byteWithBitAddr = bitsBlockAddr + m_bitNumber / 8;
			unsigned char byteWithBit = 0;
			memcpy_swapend(&byteWithBit, byteWithBitAddr, m_size, offset);
			const uint8_t bitNumberInByte = m_bitNumber % 8;
			buffer[0] = static_cast<char>(static_cast<bool>(byteWithBit & (1 << bitNumberInByte)));
		} else {
			memcpy_swapend(buffer, m_addr, m_size, offset);
		}

		return m_size;
	}

private:

	static void memcpy_swapend(void* dest, const void* src, size_t const size, void *offset) {
		const char* src2 = reinterpret_cast<const char*>(src);
		char* dest2 = reinterpret_cast<char*>(dest);
		for (size_t i = 0; i < size; i++) {
			// Endian swap - copy the end to the beginning
			dest2[i] = src2[size - 1 - i + (uint64_t)offset];
		}
	}

	static void copyFloat(char* buffer, float value) {
		memcpy_swapend(buffer, &value, sizeof(float), nullptr);
	}

	const float m_multiplier;
	const void* const m_addr;
	const uint8_t m_type_id;
	const int8_t m_digits;
	const uint8_t m_size;

	const char* const m_name;
	const char* const m_units;
	const char* const m_category;

	const bool m_isBitField;
	const uint32_t m_bitsBlockOffset; // only for bit log fields
	const uint8_t m_bitNumber; // only for bit log fields
};
}
