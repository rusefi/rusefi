/**
 * @file    sensor_type.h
 * @brief   Enumeration of sensors supported by the ECU.
 *
 * @date September 12, 2019
 * @author Matthew Kennedy, (c) 2019
 */

#pragma once

/**
 **************************************
 * SEE sensor.h ON HOW TO ADD NEW SENSOR TYPES
 **************************************
 */

enum class SensorType : unsigned char {
	Invalid = 0,
	Clt,
	Iat,

	OilPressure,

	// This is the "resolved" position, potentially composited out of the following two
	Tps1,
	// This is the first sensor
	Tps1Primary,
	// This is the second sensor
	Tps1Secondary,

	Tps2,
	Tps2Primary,
	Tps2Secondary,

	// Redundant and combined sensors for acc pedal
	AcceleratorPedal,
	AcceleratorPedalPrimary,
	AcceleratorPedalSecondary,

	// This maps to the pedal if we have one, and Tps1 if not.
	DriverThrottleIntent,

	AuxTemp1,
	AuxTemp2,

	// Leave me at the end!
	PlaceholderLast
};
