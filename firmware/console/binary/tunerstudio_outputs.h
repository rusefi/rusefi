/*
 * @file	tunerstudio_outputs.h
 * @brief	Tuner Studio connectivity configuration
 *
 * In this file the configuration of TunerStudio is defined
 *
 * @date Oct 22, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "rusefi_types.h"
#include "scaled_channel.h"
#include "tunerstudio_debug_struct.h"
#include "ts_outputs_generated.h"

enum class TsCalMode : uint8_t {
	None = 0,
	Tps1Max = 1,
	Tps1Min = 2,
	EtbKp = 3,
	EtbKi = 4,
	EtbKd = 5,
	Tps1SecondaryMax = 6,
	Tps1SecondaryMin = 7,
	Tps2Max = 8,
	Tps2Min = 9,
	Tps2SecondaryMax = 10,
	Tps2SecondaryMin = 11,
};

/**
 * todo https://github.com/rusefi/rusefi/issues/197
 * At the moment rusEFI does NOT have any code generation around TS output channels, three locations have to be changed manually
 * 1) this TunerStudioOutputChannels firmware version of the structure
 * 2) '[OutputChannels]' block in rusefi.input
 * 3) com.rusefi.core.Sensor enum in rusEFI console source code
 *
 * status update: there is progress, a portion of this struct is now generated! we inherit from generated
 * ts_outputs_s and eventually the whole thing would be generated
 *
 */
struct TunerStudioOutputChannels : ts_outputs_s {
	/* see also [OutputChannels] in rusefi.input */
	
	// temperatures
	scaled_temperature auxTemp1;             // offset 16
	scaled_temperature auxTemp2;             // offset 18

	// throttle, pedal
	scaled_percent throttlePosition;    // 20
	scaled_percent pedalPosition;       // 22
	uint16_t tpsADC;                    // 24

	// air flow/mass measurment
	scaled_voltage massAirFlowVoltage; // 26
	scaled_channel<uint16_t, PACK_MULT_MASS_FLOW> massAirFlow; // 28
	scaled_pressure manifoldAirPressure; // 30
	scaled_pressure baroPressure; // 32

	scaled_lambda lambda; // 34
	scaled_channel<uint8_t, 10> knockRetard; // 36
	uint8_t unused37;

	// misc sensors
	scaled_voltage vBatt; // 38
	scaled_pressure oilPressure; // 40
	scaled_angle vvtPositionB1I; // 42

	// Fuel math
	scaled_channel<uint16_t, 1000> chargeAirMass; // 44  cylinder airmass in mg, 0-65 grams
	scaled_fuel_mass_mg crankingFuelMass; // 46
	scaled_afr currentTargetAfr; // 48
	// This is the raw value we take from the fuel map or base fuel algorithm, before the corrections
	scaled_fuel_mass_mg fuelBase; // 50
	// Total fuel with CLT, IAT and TPS acceleration without injector lag corrections per cycle, as pulse per cycle
	scaled_fuel_mass_mg fuelRunning; // 52
	// Actual last injection time - including all compensation and injection mode
	scaled_ms actualLastInjection; // 54
	scaled_channel<uint8_t, 2> injectorDutyCycle; // 56
	scaled_channel<uint8_t, 2> veValue; // 57
	scaled_angle injectionOffset; // 58
	scaled_temperature tCharge; // 60

	// Corrections
	scaled_ms injectorLagMs; // 62
	scaled_percent iatCorrection; // 64
	scaled_percent cltCorrection; // 66
	scaled_percent baroCorrection; // 68
	uint16_t currentEnginePhase; // 70

	// Wall model AE
	scaled_ms wallFuelAmount; // 72
	scaled_channel<int16_t, 1000> wallFuelCorrection; // 74
	
	// TPS/load AE
	scaled_percent unused76; // 76
	scaled_percent deltaTps; // 78
	scaled_percent unused80; // 80
	scaled_ms tpsAccelFuel; // 82

	// Ignition
	scaled_angle ignitionAdvance; // 84
	scaled_ms sparkDwell; // 86
	scaled_percent coilDutyCycle; // 88

	// Idle & ETB
	scaled_percent idlePosition; // 90
	scaled_percent etbTarget; // 92
	scaled_percent etb1DutyCycle; // 94
	scaled_percent etb1Error; // 96

	// Fuel system
	scaled_percent fuelTankLevel; // 98
	scaled_channel<uint16_t> totalFuelConsumption; // 100
	scaled_channel<uint16_t, PACK_MULT_FUEL_FLOW> fuelFlowRate; // 102

	// Y axis values for selectable tables
	scaled_channel<uint16_t, 100> veTableYAxis;  // 104
	scaled_channel<uint16_t, 100> afrTableYAxis; // 106

	// Knock
	scaled_channel<float> knockLevel; // 108

	// Mode, firmware, protocol, run time
	scaled_channel<uint32_t> timeSeconds; // 112
	scaled_channel<uint32_t> engineMode; // 116
	scaled_channel<uint32_t> firmwareVersion; // 120
	// todo: this not needed in light of TS_SIGNATURE but rusEFI console still uses it. Need to migrate
	// rusEFI console from TS_FILE_VERSION to TS_SIGNATURE :(

	uint32_t tsConfigVersion; // 124

	// These two fields indicate to TS that we'd like to set a particular field to a particular value
	// We use a maintainConstantValue in TS for each field we'd like to set, like this:
	//		maintainConstantValue = tpsMax, { (calibrationMode == 1 ) ? calibrationValue : tpsMax }
	//		maintainConstantValue = tpsMin, { (calibrationMode == 2 ) ? calibrationValue : tpsMin }
	// When the mode is set to a particular value, TS will copy the calibrationValue in to the specified field.
	//
	// With this simple construct, the ECU can send any number of internally computed configuration fields
	// back to TunerStudio, getting around the problem of setting values on the controller without TS's knowledge.
	// The ECU simply has to sequentially set a mode/value, wait briefly, then repeat until all the values
	// it wants to send have been sent.
	float calibrationValue;	// 128
	TsCalMode calibrationMode; // 132
	uint8_t padding[1]; // 133

	// Values used for load axes for fuel/ign tables
	// These may or may not be the same value, depending on mode
	scaled_channel<uint16_t, 100> fuelingLoad; // 134
	scaled_channel<uint16_t, 100> ignitionLoad; // 136

	// we want a hash of engineMake+engineCode+vehicleName in the log file in order to match TS logs to rusEFI Online tune
	scaled_channel<uint16_t> engineMakeCodeNameCrc16; // 138
	// Errors
	scaled_channel<uint32_t> totalTriggerErrorCounter; // 140
	int orderingErrorCounter; // 144
	scaled_channel<uint16_t> warningCounter; // 148
	scaled_channel<uint16_t> lastErrorCode; // 150
	int16_t recentErrorCodes[8]; // 152-166

	// Debug
	scaled_channel<float> debugFloatField1; // 168
	scaled_channel<float> debugFloatField2;
	scaled_channel<float> debugFloatField3;
	scaled_channel<float> debugFloatField4;
	scaled_channel<float> debugFloatField5;
	scaled_channel<float> debugFloatField6;
	scaled_channel<float> debugFloatField7;
	scaled_channel<uint32_t> debugIntField1;
	scaled_channel<uint32_t> debugIntField2;
	scaled_channel<uint32_t> debugIntField3;
	scaled_channel<uint16_t> debugIntField4;
	scaled_channel<uint16_t> debugIntField5; // 210

	// accelerometer
	scaled_percent accelerationX; // 212
	scaled_percent accelerationY; // 214

	// EGT
	uint16_t egtValues[EGT_CHANNEL_COUNT] ; // 216

	scaled_percent throttle2Position;    // 232

	scaled_voltage rawTps1Primary;		// 234
	scaled_voltage rawPpsPrimary;		// 236
	scaled_voltage rawClt;				// 238
	scaled_voltage rawIat;				// 240
	scaled_voltage rawOilPressure;		// 242

	scaled_channel<uint16_t> tuneCrc16; // 244

	scaled_channel<uint8_t> unusedAt246;

	scaled_channel<uint8_t> tcuCurrentGear; // 247

	scaled_voltage rawPpsSecondary;		// 248

	scaled_channel<int8_t> knockLevels[12];		// 250

	scaled_channel<uint8_t> tcuDesiredGear; // 262
	scaled_channel<uint8_t, 2> flexPercent;		// 263

	scaled_voltage rawIdlePositionSensor;	// 264
	scaled_voltage rawWastegatePositionSensor;	// 266

	scaled_percent wastegatePosition;	// 268
	scaled_percent idlePositionSensor;	// 270

	scaled_voltage rawLowFuelPressure; // 272
	scaled_voltage rawHighFuelPressure; // 274

	scaled_pressure lowFuelPressure;	// 276
	scaled_high_pressure highFuelPressure;	// 278

	scaled_lambda targetLambda; // 280
	scaled_afr airFuelRatio; // 282

	scaled_ms VssAcceleration; //284

	scaled_lambda lambda2; // 286
	scaled_afr airFuelRatio2; // 288

	scaled_angle vvtPositionB1E; // 290
	scaled_angle vvtPositionB2I; // 292
	scaled_angle vvtPositionB2E; // 294

	scaled_percent fuelTrim[2];	// 296 & 298

	scaled_voltage rawTps1Secondary;	// 300
	scaled_voltage rawTps2Primary;		// 302
	scaled_voltage rawTps2Secondary;	// 304

	scaled_channel<uint16_t> knockCount;// 306

	scaled_percent accelerationZ; // 308
	scaled_percent accelerationRoll; // 310
	scaled_percent accelerationYaw; // 312

	scaled_channel<int8_t> vvtTargets[4]; // 314
	scaled_channel<uint16_t> turboSpeed; // 318

	uint8_t unusedAtTheEnd[18]; // we have some unused bytes to allow compatible TS changes

	// Temporary - will remove soon
	TsDebugChannels* getDebugChannels() {
		return reinterpret_cast<TsDebugChannels*>(&debugFloatField1);
	}

	/* see also [OutputChannels] in rusefi.input */
	/* see also TS_OUTPUT_SIZE in rusefi_config.txt */
};

extern TunerStudioOutputChannels tsOutputChannels;
