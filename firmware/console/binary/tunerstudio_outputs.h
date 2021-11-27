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
