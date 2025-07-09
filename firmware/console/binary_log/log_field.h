#pragma once

#include "efi_scaled_channel.h"
#include "rusefi_types.h"
#include <concepts>
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
		, m_type(resolveBuiltInNumberType<TValue>())
		, m_digits(digits)
		, m_size(sizeForType<resolveBuiltInNumberType<TValue>()>())
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
		, m_type(resolveBuiltInNumberType<TValue>())
		, m_digits(digits)
		, m_size(sizeForType<resolveBuiltInNumberType<TValue>()>())
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
		U08 = 0,
		S08 = 1,
		U16 = 2,
		S16 = 3,
		U32 = 4,
		S32 = 5,
		S64 = 6,
		F32 = 7,
		U64 = 8,
		F64 = 9,
		unsupported = static_cast<uint8_t>(-1)
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
	static consteval Type resolveBuiltInNumberType() {
		using enum Type;
		using CleanType = std::remove_const_t<T>;
		constexpr auto resolvedType{[](){
			if      constexpr (std::same_as<CleanType, float>)  { return F32; }
			else if constexpr (std::same_as<CleanType, double>) { return F64; }
			else if constexpr (std::is_integral_v<CleanType>) {
				if constexpr (std::is_signed_v<CleanType>) {
					if      constexpr (sizeof(CleanType) == 1) { return S08; }
					else if constexpr (sizeof(CleanType) == 2) { return S16; }
					else if constexpr (sizeof(CleanType) == 4) { return S32; }
					else if constexpr (sizeof(CleanType) == 8) { return S64; }
				} else {
					if      constexpr (sizeof(CleanType) == 1) { return U08; }
					else if constexpr (sizeof(CleanType) == 2) { return U16; }
					else if constexpr (sizeof(CleanType) == 4) { return U32; }
					else if constexpr (sizeof(CleanType) == 8) { return U64; }
				}
			}
			else { return unsupported; }
		}()};
		static_assert(resolvedType != unsupported, "Type was not recognized as supported built in numeric type");
		return resolvedType;
	}

	template<Type t>
	consteval size_t sizeForType() {
		constexpr auto s{[]{
			switch (t) {
				using enum Type;
				case U08: case S08: return 1;
				case U16: case S16: return 2;
				case U32: case S32: case F32: return 4;
				case U64: case S64: case F64: return 8;
				default: return 0;
			}
		}()};
		static_assert(s != 0, "Can not resolve type, check enum for new values that were left unhandled");
		return s;
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
