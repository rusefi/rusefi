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
 * While changing SensorType make sure to change s_sensorNames
 * TODO: SensorType to s_sensorNames mapping has to be done by code generation! We already have relevant tools
 * maybe we just need a better naming convention?
 **************************************
 */
enum class SensorType : unsigned char {
	Invalid = 0,
	Clt,
	Iat,
	Rpm,
	Map,
	Maf,

	OilPressure,

	FuelPressureLow,
	FuelPressureHigh,
	FuelPressureInjector,

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

	Lambda,

	WastegatePosition,
	IdlePosition,

	// Leave me at the end!
	PlaceholderLast
};
