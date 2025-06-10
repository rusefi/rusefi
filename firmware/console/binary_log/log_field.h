#pragma once

#include <cstdint>
#include <type_traits>

struct Writer;
class LogField {
public:
	// Scaled channels, memcpys data directly and describes format in header
	template <typename TValue, int TMult, int TDiv>
	consteval LogField(const scaled_channel<TValue, TMult, TDiv>& toRead,
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
	consteval LogField(TValue& toRead,
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
	consteval LogField(
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
		U08 = 0,
		S08 = 1,
		U16 = 2,
		S16 = 3,
		U32 = 4,
		S32 = 5,
		S64 = 6,
		F32 = 7,
	};

	consteval size_t getSize() const {
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
	static consteval Type resolveType();

	static consteval size_t sizeForType(Type t) {
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

template<typename T>
struct TypeResolver;

template<>
struct TypeResolver<uint8_t> {
	static constexpr LogField::Type value = LogField::Type::U08;
};

template<>
struct TypeResolver<int8_t> {
	static constexpr LogField::Type value = LogField::Type::S08;
};

template<>
struct TypeResolver<uint16_t> {
	static constexpr LogField::Type value = LogField::Type::U16;
};

template<>
struct TypeResolver<int16_t> {
	static constexpr LogField::Type value = LogField::Type::S16;
};

template<>
struct TypeResolver<uint32_t> {
	static constexpr LogField::Type value = LogField::Type::U32;
};

template<>
struct TypeResolver<int32_t> {
	static constexpr LogField::Type value = LogField::Type::S32;
};

template<>
struct TypeResolver<float> {
	static constexpr LogField::Type value = LogField::Type::F32;
};

#if EFI_PROD_CODE
// we allow both 'int' and 'int32_t' just to allow extra flexibility in headers
// https://stackoverflow.com/questions/55782246/why-is-uint32-t-typedeffed-to-unsigned-long-on-arm-none-eabi-gcc-and-how-to
template<>
struct TypeResolver<int> {
	static constexpr LogField::Type value = LogField::Type::S32;
};
#endif

template<typename T>
consteval LogField::Type LogField::resolveType() {
	return TypeResolver<std::remove_cv_t<T>>::value;
}
