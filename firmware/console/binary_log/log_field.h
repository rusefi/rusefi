#pragma once

#include "efi_scaled_channel.h"
#include "rusefi_types.h"
#include <cstdint>
#include <cstddef>

struct Writer;
class LogField {
public:
	// Scaled channels, memcpys data directly and describes format in header
	template <typename TValue, int TMult, int TDiv>
	constexpr LogField(const scaled_channel<TValue, TMult, TDiv>& toRead,
			   const char* name, const char* units, int8_t digits, const char* category = "none")
		: m_multiplier(float(TDiv) / TMult)
		, m_addr(toRead.getFirstByteAddr())
		, m_type(resolveType<TValue>())
		, m_digits(digits)
		, m_size(sizeForType(resolveType<TValue>()))
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
	constexpr LogField(TValue& toRead,
			   const char* name, const char* units, int8_t digits, const char* category = "none")
		: m_multiplier(1)
		, m_addr(&toRead)
		, m_type(resolveType<TValue>())
		, m_digits(digits)
		, m_size(sizeForType(resolveType<TValue>()))
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
	constexpr LogField(
		TValue& toRead,
		const uint32_t bitsBlockOffset,
		const uint8_t bitNumber,
		const char* name,
		const char* units,
		const char* category = "none"
	): m_multiplier(1)
		, m_addr(&toRead)
		, m_type(Type::U08)
		, m_digits(0)
		, m_size(1)
		, m_name(name)
		, m_units(units)
		, m_category(category)
		, m_isBitField(true)
		, m_bitsBlockOffset(bitsBlockOffset)
		, m_bitNumber(bitNumber)
	{}

	enum class Type : uint8_t {
	  // NOTE THAT Ordinals are part of logging serialization as described in
	  // https://www.efianalytics.com/TunerStudio/docs/MLG_Binary_LogFormat_2.0.pdf
		U08 = 0,
		S08 = 1,
		U16 = 2,
		S16 = 3,
		U32 = 4,
		S32 = 5,
		S64 = 6,
		F32 = 7,
	};

	constexpr size_t getSize() const {
		return m_size;
	}

	// Write the header data describing this field.
	// Returns the number of bytes written.
	size_t writeHeader(Writer& outBuffer) const;

	// Write the field's data to the buffer.
	// Returns the number of bytes written.
	size_t writeData(char* buffer, void *offset) const;

#if EFI_UNIT_TEST
  bool needsEngineOffsetHack(size_t size) const {
    // low addresses are offsets without engine reference
    return (intptr_t)m_addr < size;
  }
#endif

private:
	template<typename T>
	static constexpr Type resolveType();

	static constexpr size_t sizeForType(Type t) {
		switch (t) {
			case Type::U08:
			case Type::S08:
				return 1;
			case Type::U16:
			case Type::S16:
				return 2;
			default:
				// float, uint32, int32
				return 4;
		}
	}

	const float m_multiplier;
	const void* const m_addr;
	const Type m_type;
	const int8_t m_digits;
	const uint8_t m_size;

	const char* const m_name;
	const char* const m_units;
	const char* const m_category;

	const bool m_isBitField;
	const uint32_t m_bitsBlockOffset; // only for bit log fields
	const uint8_t m_bitNumber; // only for bit log fields
};

template<>
constexpr LogField::Type LogField::resolveType<const uint8_t>() {
	return Type::U08;
}

template<>
constexpr LogField::Type LogField::resolveType<uint8_t>() {
	return Type::U08;
}

template<>
constexpr LogField::Type LogField::resolveType<int8_t>() {
	return Type::S08;
}

template<>
constexpr LogField::Type LogField::resolveType<const int8_t>() {
	return Type::S08;
}

template<>
constexpr LogField::Type LogField::resolveType<uint16_t>() {
	return Type::U16;
}


template<>
constexpr LogField::Type LogField::resolveType<const uint16_t>() {
	return Type::U16;
}

template<>
constexpr LogField::Type LogField::resolveType<int16_t>() {
	return Type::S16;
}

template<>
constexpr LogField::Type LogField::resolveType<uint32_t>() {
	return Type::U32;
}

#if EFI_PROD_CODE
// we allow both 'int' and 'int32_t' just to allow extra flexibility in headers
// https://stackoverflow.com/questions/55782246/why-is-uint32-t-typedeffed-to-unsigned-long-on-arm-none-eabi-gcc-and-how-to
template<>
constexpr LogField::Type LogField::resolveType<int>() {
	return Type::S32;
}
#endif

template<>
constexpr LogField::Type LogField::resolveType<int32_t>() {
	return Type::S32;
}

template<>
constexpr LogField::Type LogField::resolveType<float>() {
	return Type::F32;
}

template<>
constexpr LogField::Type LogField::resolveType<const float>() {
	return Type::F32;
}
