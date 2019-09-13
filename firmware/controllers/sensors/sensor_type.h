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

	// This is the "resolved" position, potentially composited out of the following two
	Tps1,
	// This is the first sensor
	Tps1Primary,
	// This is the second sensor
	Tps1Secondary,

	// Leave me at the end!
	PlaceholderLast
};
