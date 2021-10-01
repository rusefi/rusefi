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
 * While changing SensorType make sure to change s_sensorNames
 * TODO: SensorType to s_sensorNames mapping has to be done by code generation! We already have relevant tools
 * maybe we just need a better naming convention?
 **************************************
 */
enum class SensorType : unsigned char {
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

	MapFast = 36,
	MapSlow = 37,

	// Leave me at the end!
	PlaceholderLast = 36,
};
