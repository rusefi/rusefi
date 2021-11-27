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
 * three locations have to be changed manually
 * 1) we inherit from generated ts_outputs_s based on output_channels.txt
 * 2) '[OutputChannels]' block in rusefi.input
 * 3) com.rusefi.core.Sensor enum in rusEFI console source code
 *
 * see also [OutputChannels] in rusefi.input
 * see also TS_OUTPUT_SIZE in rusefi_config.txt
 */
struct TunerStudioOutputChannels : ts_outputs_s {
	// Temporary - will remove soon
	TsDebugChannels* getDebugChannels() {
		return reinterpret_cast<TsDebugChannels*>(&debugFloatField1);
	}

};

extern TunerStudioOutputChannels tsOutputChannels;
