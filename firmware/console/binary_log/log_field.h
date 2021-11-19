#pragma once

#include "scaled_channel.h"
#include <cstdint>
#include <cstddef>

struct Writer;
class LogField {
public:
	template <typename TValue, int TMult, int TDiv>
	constexpr LogField(const scaled_channel<TValue, TMult, TDiv>& toRead,
			   const char* name, const char* units, int8_t digits)
		: m_multiplier(float(TDiv) / TMult)
		, m_addr(toRead.getFirstByteAddr())
		, m_type(resolveType<TValue>())
		, m_digits(digits)
		, m_size(sizeForType(resolveType<TValue>()))
		, m_name(name)
		, m_units(units)
	{
	}

	enum class Type : uint8_t {
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
	void writeHeader(Writer& outBuffer) const;

	// Write the field's data to the buffer.
	// Returns the number of bytes written.
	size_t writeData(char* buffer) const;

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
	const char* const m_addr;
	const Type m_type;
	const int8_t m_digits;
	const uint8_t m_size;

	const char* const m_name;
	const char* const m_units;
};

template<>
constexpr LogField::Type LogField::resolveType<uint8_t>() {
	return Type::U08;
}

template<>
constexpr LogField::Type LogField::resolveType<int8_t>() {
	return Type::S08;
}

template<>
constexpr LogField::Type LogField::resolveType<uint16_t>() {
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

template<>
constexpr LogField::Type LogField::resolveType<int32_t>() {
	return Type::S32;
}

template<>
constexpr LogField::Type LogField::resolveType<float>() {
	return Type::F32;
}
