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
	 * oh, and there is instantMap which is third thing not part of sensor framework
	 * this sensor is Post-TB MAP Sensor
	 * for ThrottleInletPressure see ThrottleInletPressure
	 */
	Map,
	Maf,

	AmbientTemperature,

	OilPressure,
	OilTemperature,

	FuelPressureLow, // in kPa
	FuelPressureHigh, // in kPa
	// ProxySensor for one of the above
	FuelPressureInjector,

	FuelTemperature,

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
	// pedal according to only first physical sensor
	AcceleratorPedalPrimary,
	// pedal according to only second physical sensor
	AcceleratorPedalSecondary,

	// This maps to the pedal if we have one, and Tps1 if not.
	DriverThrottleIntent,

	AuxTemp1,
	AuxTemp2,

  // this becomes AFR once we multiply by STOICH_RATIO
	Lambda1,
	Lambda2,
	SmoothedLambda1,
	SmoothedLambda2,

	WastegatePosition,
	IdlePosition,

	FuelEthanolPercent,

	BatteryVoltage,

	BarometricPressure,

	FuelLevel,

	VehicleSpeed,
	WheelSlipRatio,

	TurbochargerSpeed,

	// Fast MAP is synchronous to crank angle - user selectable phase/window
	MapFast,
	// Slow MAP is asynchronous - not synced to anything, normal analog sampling
	// MAP decoding happens only that often thus this is NOT raw MAP as flows from ADC
	MapSlow,

	InputShaftSpeed,

	EGT1,
	EGT2,
	EGT3,
	EGT4,
	EGT5,
	EGT6,
	EGT7,
	EGT8,

	Maf2,	// Second bank MAF sensor

	// todo: finish implementation?
	Map2,
	MapSlow2,
	MapFast2,

	// Pressure sensor after compressor, before intercooler
	CompressorDischargePressure,
	CompressorDischargeTemperature,

	// Pressure sensor before the throttle, after any turbo/etc
	ThrottleInletPressure,

	DetectedGear,

	RangeInput1,
	RangeInput2,
	RangeInput3,
	RangeInput4,
	RangeInput5,
	RangeInput6,

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
	LuaGauge3,
	LuaGauge4,
	LuaGauge5,
	LuaGauge6,
	LuaGauge7,
	LuaGauge8,

    IgnKeyVoltage,

    DashOverrideRpm,
    DashOverrideVehicleSpeed,
    DashOverrideClt,
    DashOverrideBatteryVoltage,

    AcPressure,

    GearboxPositionSensor,

	AuxLinear1,
	AuxLinear2,
	AuxLinear3,
	AuxLinear4,

	// frequency sensors
	AuxSpeed1,
	AuxSpeed2,

	// Let's always have all auxiliary sensors at the end - please add specific sensors above auxiliary

	// Leave me at the end!
	PlaceholderLast,
};
