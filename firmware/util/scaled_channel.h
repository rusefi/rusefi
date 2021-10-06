/*
 * @file	scaled_channel.h
 * @brief	Scaled channel storage for binary formats
 *
 * Storage of values (floating point, usually) scaled in integer storage, for transmission over the wire.
 * This includes Tunerstudio serial/USB, and CAN.
 *
 * @date Feb 24, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#pragma once

#include <cstdint>
#include <type_traits>

#include "rusefi_generated.h"

// This class lets us transparently store something at a ratio inside an integer type
// Just use it like a float - you can read and write to it, like this:
// scaled_channel<uint8_t, 10> myVar;
// myVar = 2.4f;	// converts to an int, stores 24
// float x = myVar; // converts back to float, returns 2.4f
template <typename T, int mult = 1>
class scaled_channel {
	using TSelf = scaled_channel<T, mult>;

public:
	constexpr scaled_channel() : m_value(static_cast<T>(0)) { }
	constexpr scaled_channel(float val)
		: m_value(val * mult)
	{
	}

	// Allow reading back out as a float (note: this may be lossy!)
	constexpr operator float() const {
		return m_value / (float)mult;
	}

    // Postfix increment operator
	// only enable if:
	//  - base type T is an integral type (integer)
	//  - multiplier is equal to 1
	void operator++(int) {
		static_assert(mult == 1, "Increment operator only supported for non-scaled integer types");
		static_assert(std::is_integral_v<T>, "Increment operator only supported for non-scaled integer types");

		m_value++;
	}

	constexpr const char* getFirstByteAddr() const {
		return &m_firstByte;
	}

private:
	union {
		T m_value;
		char m_firstByte;
	};
};

// We need to guarantee that scaled values containing some type are the same size
// as that underlying type.  We rely on the class only having a single member for
// this trick to work.
static_assert(sizeof(scaled_channel<uint8_t>) == 1);
static_assert(sizeof(scaled_channel<uint16_t>) == 2);
static_assert(sizeof(scaled_channel<uint32_t>) == 4);
static_assert(sizeof(scaled_channel<float>) == 4);

// Common scaling options - use these if you can!
using scaled_temperature = scaled_channel<int16_t, PACK_MULT_TEMPERATURE>;	// +-327 deg C at 0.01 deg resolution
using scaled_ms = scaled_channel<int16_t, PACK_MULT_MS>;				// +- 100ms at 0.003ms precision
using scaled_percent = scaled_channel<int16_t, PACK_MULT_PERCENT>;		// +-327% at 0.01% resolution
using scaled_pressure = scaled_channel<uint16_t, PACK_MULT_PRESSURE>;		// 0-2000kPa (~300psi) at 0.03kPa resolution
using scaled_high_pressure = scaled_channel<uint16_t, PACK_MULT_HIGH_PRESSURE>;	// 0-6553 bar (~95k psi) at 0.1 bar resolution
using scaled_angle = scaled_channel<int16_t, PACK_MULT_ANGLE>;			// +-655 degrees at 0.02 degree resolution
using scaled_voltage = scaled_channel<uint16_t, PACK_MULT_VOLTAGE>;		// 0-65v at 1mV resolution
using scaled_afr = scaled_channel<uint16_t, PACK_MULT_AFR>;			// 0-65afr at 0.001 resolution
using scaled_lambda = scaled_channel<uint16_t, PACK_MULT_LAMBDA>;	// 0-6.5 lambda at 0.0001 resolution
using scaled_fuel_mass_mg = scaled_channel<uint16_t, PACK_MULT_FUEL_MASS>;	// 0 - 655.35 milligrams, 0.01mg resolution
