#pragma once

#include <cstdint>
#include <cstring>
#include <type_traits>

#include <rusefi/scaled_channel.h>

#include "writer.h"
#include "mlg_types.h"

// Direct-reference constructors also support local values in unit tests.
#if EFI_UNIT_TEST
#define LOG_FIELD_CONSTNESS_SPECIFIER_METHODS constexpr
#else
#define LOG_FIELD_CONSTNESS_SPECIFIER_METHODS consteval
#endif
#define LOG_FIELD_CONSTNESS_SPECIFIER_STORAGE const constinit

// [tag:live_data_sd_mapping] Resolve generated sources at compile time on the ECU,
// and at each read on hosts, where Engine instances and mocks can be replaced.
#define MLG_SOURCE(...) ::MLG::Entries::makeSource<[]() -> auto& { return (__VA_ARGS__); }>()

namespace MLG::Entries {
using namespace MLG;

class FieldSource {
public:
	constexpr explicit FieldSource(const void* address) : m_address(address) {}
#if !EFI_PROD_CODE
	constexpr explicit FieldSource(const void* (*resolve)()) : m_address(nullptr), m_resolve(resolve) {}
#endif

	constexpr const void* get() const {
#if !EFI_PROD_CODE
		if (m_resolve) {
			return m_resolve();
		}
#endif
		return m_address;
	}

private:
	const void* m_address;
#if !EFI_PROD_CODE
	const void* (*m_resolve)() = nullptr;
#endif
};

// Retain the value type for metadata generation without evaluating the source.
template<typename T>
struct Source : FieldSource {
	using FieldSource::FieldSource;
};

template<typename T>
constexpr const void* fieldAddress(const T& value) {
	return &value;
}

template<typename T, int Mult, int Div>
constexpr const void* fieldAddress(const scaled_channel<T, Mult, Div>& value) {
	return value.getFirstByteAddr();
}

template<auto Getter>
consteval auto makeSource() {
	static_assert(std::is_lvalue_reference_v<decltype(Getter())>);
	using T = std::remove_cvref_t<decltype(Getter())>;
#if EFI_PROD_CODE
	return Source<T>{fieldAddress(Getter())};
#else
	return Source<T>{+[]() -> const void* { return fieldAddress(Getter()); }};
#endif
}

class Field {
public:
	template<typename T, typename... Args>
		requires (!std::is_base_of_v<FieldSource, T>)
	LOG_FIELD_CONSTNESS_SPECIFIER_METHODS Field(T& value, Args... args)
		: Field(Source<std::remove_cv_t<T>>{fieldAddress(value)}, args...) {}

	// Scaled channels, memcpys data directly and describes format in header
	template <typename TValue, int TMult, int TDiv>
	LOG_FIELD_CONSTNESS_SPECIFIER_METHODS Field(Source<scaled_channel<TValue, TMult, TDiv>> source,
			   const char* name, const char* units, int8_t digits, const char* category = "none", float multiplier = 1, float translation = 0)
		: m_multiplier(float(TDiv) / TMult * multiplier)
		, m_translation(translation)
		, m_source(source)
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

	// Non-scaled channel, works for arithmetic types and enums stored as their underlying integer type.
	// [tag:live_data_output_scale] engineering = (raw + translation) * multiplier.
	// The header stores the conversion, including raw packed offsets; writeData keeps storage unchanged.
	template <typename TValue, typename = typename std::enable_if<std::is_arithmetic_v<TValue> || std::is_enum_v<TValue>>::type>
	LOG_FIELD_CONSTNESS_SPECIFIER_METHODS Field(Source<TValue> source,
			   const char* name, const char* units, int8_t digits, const char* category = "none", float multiplier = 1, float translation = 0)
		: m_multiplier(multiplier)
		, m_translation(translation)
		, m_source(source)
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
		Source<TValue> source,
		const uint32_t bitsBlockOffset,
		const uint8_t bitNumber,
		const char* name,
		const char* units,
		const char* category = "none"
	): m_multiplier(1)
		, m_translation(0)
		, m_source(source)
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
	constexpr const void* getAddr() const { return m_source.get(); }
	constexpr const char* getName() const { return m_name; }
	constexpr const char* getUnits() const { return m_units; }
	constexpr const char* getCategory() const { return m_category; }
	constexpr float getMultiplier() const { return m_multiplier; }
	constexpr float getTranslation() const { return m_translation; }
	constexpr int8_t getDigits() const { return m_digits; }
	constexpr uint8_t getTypeId() const { return m_type_id; }
	constexpr bool isBitField() const { return m_isBitField; }

	// Reads the field's current value, applies translation/scale or bit extraction, and returns it as a double.
	double readValueAsDouble() const {
		using namespace Types::Field;
		if (m_isBitField) {
			const char* const bitsBlockAddr = static_cast<const char*>(getAddr()) + m_bitsBlockOffset;
			const char* const byteWithBitAddr = bitsBlockAddr + m_bitNumber / 8;
			unsigned char byteWithBit = 0;
			memcpy_swapend(&byteWithBit, byteWithBitAddr, 1);
			const uint8_t bitNumberInByte = m_bitNumber % 8;
			return (byteWithBit & (1 << bitNumberInByte)) ? 1.0 : 0.0;
		}
		uint8_t raw[8] = {0};
		// Note: writeData swaps endianness for the binary log; here we want native value.
		const char* src = static_cast<const char*>(getAddr());
		for (size_t i = 0; i < m_size; i++) {
			raw[i] = static_cast<uint8_t>(src[i]);
		}
		double rawValue = 0;
		switch (static_cast<Scalar>(m_type_id)) {
			case Scalar::U08: rawValue = *reinterpret_cast<uint8_t*>(raw); break;
			case Scalar::S08: rawValue = *reinterpret_cast<int8_t*>(raw); break;
			case Scalar::U16: rawValue = *reinterpret_cast<uint16_t*>(raw); break;
			case Scalar::S16: rawValue = *reinterpret_cast<int16_t*>(raw); break;
			case Scalar::U32: rawValue = *reinterpret_cast<uint32_t*>(raw); break;
			case Scalar::S32: rawValue = *reinterpret_cast<int32_t*>(raw); break;
			case Scalar::S64: rawValue = static_cast<double>(*reinterpret_cast<int64_t*>(raw)); break;
			case Scalar::F32: rawValue = *reinterpret_cast<float*>(raw); break;
			default: rawValue = 0; break;
		}
		return (rawValue + m_translation) * m_multiplier;
	}

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

		// Offset 50, length 4 = shift in raw units, before scaling
		copyFloat(buffer + 50, m_translation);

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
	size_t writeData(char* buffer) const {
		if (m_isBitField) {
			const char* const bitsBlockAddr = static_cast<const char*>(getAddr()) + m_bitsBlockOffset;
			const char* const byteWithBitAddr = bitsBlockAddr + m_bitNumber / 8;
			unsigned char byteWithBit = 0;
			memcpy_swapend(&byteWithBit, byteWithBitAddr, m_size);
			const uint8_t bitNumberInByte = m_bitNumber % 8;
			buffer[0] = static_cast<char>(static_cast<bool>(byteWithBit & (1 << bitNumberInByte)));
		} else {
			memcpy_swapend(buffer, getAddr(), m_size);
		}

		return m_size;
	}

private:

	static void memcpy_swapend(void* dest, const void* src, size_t const size) {
		const char* src2 = reinterpret_cast<const char*>(src);
		char* dest2 = reinterpret_cast<char*>(dest);
		for (size_t i = 0; i < size; i++) {
			// Endian swap - copy the end to the beginning
			dest2[i] = src2[size - 1 - i];
		}
	}

	static void copyFloat(char* buffer, float value) {
		memcpy_swapend(buffer, &value, sizeof(float));
	}

	const float m_multiplier;
	const float m_translation;
	const FieldSource m_source;
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
