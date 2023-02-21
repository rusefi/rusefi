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
enum class SensorType : unsigned char {
	Invalid, // we need indeces for Lua consumers. At the moment we still do not expose constants into Lua :(
	Clt, // 1
	Iat,
	Rpm,
	/**
	 * This value is result of averaging within user-defined window
	 * See also MapFast, MapSlow
	 */
	Map,
	Maf,

	OilPressure, // 6

	FuelPressureLow, // in kPa
	FuelPressureHigh, // in kPa
	FuelPressureInjector,

	// This is the "resolved" position, potentially composited out of the following two
	Tps1, // 10
	// Combined analog redundant sensor
	AnalogTps1,
	// First analog sensor
	Tps1Primary,
	// Second analog sensor
	Tps1Secondary,
	// alternative physical implementation - digital and CRC protected, natively redundant
	SentTps1,

	Tps2,
	Tps2Primary,
	Tps2Secondary,

	// Redundant and combined sensors for acc pedal
	AcceleratorPedal,
	AcceleratorPedalPrimary,
	AcceleratorPedalSecondary,

	// This maps to the pedal if we have one, and Tps1 if not.
	DriverThrottleIntent,

	AuxTemp1, // 22
	AuxTemp2,

	Lambda1,
	Lambda2,

	WastegatePosition,
	IdlePosition,

	FuelEthanolPercent,

	BatteryVoltage,

	BarometricPressure,

	FuelLevel,

	VehicleSpeed,

	TurbochargerSpeed,

	// Fast MAP is synchronous to crank angle - user selectable phase/window
	MapFast,
	// Slow MAP is asynchronous - not synced to anything, normal analog sampling
	// MAP decoding happens only that often thus this is NOT raw MAP as flows from ADC
	MapSlow,

	InputShaftSpeed,

	EGT1,
	EGT2,

	Maf2,	// Second bank MAF sensor

	Map2,
	MapSlow2,
	MapFast2,

	// analog voltage inputs for Lua
	AuxAnalog1,
	AuxAnalog2,
	AuxAnalog3,
	AuxAnalog4,
	AuxAnalog5,
	AuxAnalog6,
	AuxAnalog7,
	AuxAnalog8,

	LuaGauge1,
	LuaGauge2,

	AuxLinear1,
	AuxLinear2,

	// frequency sensors
	AuxSpeed1,
	AuxSpeed2,

	// Let's always have all auxiliary sensors at the end - please add specific sensors above auxiliary

	// Leave me at the end!
	PlaceholderLast,
};
