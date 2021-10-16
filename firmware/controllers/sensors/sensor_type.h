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
	Invalid = 0,
	Clt = 1,
	Iat = 2,
	Rpm = 3,
	Map = 4,
	Maf = 5,

	OilPressure = 6,

	FuelPressureLow = 7, // in kPa
	FuelPressureHigh = 8, // in kPa
	FuelPressureInjector = 9,

	// This is the "resolved" position, potentially composited out of the following two
	Tps1 = 10,
	// This is the first sensor
	Tps1Primary = 11,
	// This is the second sensor
	Tps1Secondary = 12,

	Tps2 = 13,
	Tps2Primary = 14,
	Tps2Secondary = 15,

	// Redundant and combined sensors for acc pedal
	AcceleratorPedal = 16,
	AcceleratorPedalPrimary = 17,
	AcceleratorPedalSecondary = 18,

	// This maps to the pedal if we have one, and Tps1 if not.
	DriverThrottleIntent = 19,

	AuxTemp1 = 20,
	AuxTemp2 = 21,

	Lambda1 = 22,
	Lambda2 = 23,

	WastegatePosition = 24,
	IdlePosition = 25,

	FuelEthanolPercent = 26,

	BatteryVoltage = 27,

	BarometricPressure = 28,

	FuelLevel = 29,

	Aux1 = 30,
	Aux2 = 31,
	Aux3 = 32,
	Aux4 = 33,

	VehicleSpeed = 34,

	TurbochargerSpeed = 35,

	// Fast MAP is synchronous to crank angle - user selectable phase/window
	MapFast = 36,
	// Slow MAP is asynchronous - not synced to anything, normal analog sampling
	MapSlow = 37,

	// Leave me at the end!
	PlaceholderLast = 38,
} SensorType;
