/**
 * @file    sensor_type.h
 * @brief   Enumeration of sensors supported by the ECU.
 *
 * @date September 12, 2019
 * @author Matthew Kennedy, (c) 2019
 */

#pragma once

#define VBAT_FALLBACK_VALUE 12

/**
 **************************************
 * SEE sensor.h ON HOW TO ADD NEW SENSOR TYPES
 **************************************
 */
typedef enum  __attribute__ ((__packed__)) {
	Invalid, // we need indeces for Lua consumers. At the moment we still do not expose constants into Lua :(
	Clt, // 1
	Iat,
	Rpm,
	Map,
	Maf,

	OilPressure, // 6

	FuelPressureLow, // in kPa
	FuelPressureHigh, // in kPa
	FuelPressureInjector,

	// This is the "resolved" position, potentially composited out of the following two
	Tps1, // 10
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

	Lambda1,
	Lambda2,

	WastegatePosition,
	IdlePosition,

	FuelEthanolPercent,

	BatteryVoltage,

	BarometricPressure,

	FuelLevel,

	Aux1,
	Aux2,
	Aux3,
	Aux4,

	VehicleSpeed,

	TurbochargerSpeed,

	// Fast MAP is synchronous to crank angle - user selectable phase/window
	MapFast,
	// Slow MAP is asynchronous - not synced to anything, normal analog sampling
	MapSlow,

	// Leave me at the end!
	PlaceholderLast,
} SensorType;
