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

#include <rusefi/scaled_channel.h>
#include "rusefi_generated.h"

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
