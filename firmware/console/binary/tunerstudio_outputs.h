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
#include "efi_scaled_channel.h"
#include "output_channels_generated.h"

#ifndef WITH_TS_STATE
// do we ever see a case of compiling without TSOutputChannels? Shall we wipe out all '#if EFI_TUNER_STUDIO' or replace with '#if WITH_TS_STATE'
#define WITH_TS_STATE TRUE
#endif

/**
 * todo https://github.com/rusefi/rusefi/issues/197
 * three locations have to be changed manually
 * 1) we inherit from generated ts_outputs_s based on output_channels.txt
 * 2) '[OutputChannels]' block in rusefi.input
 * 3) com.rusefi.core.Sensor enum in rusEFI console source code
 * 4) static constexpr LogField fields[] SD card logging
 *
 * see also [OutputChannels] in rusefi.input
 * see also TS_OUTPUT_SIZE in rusefi_config.txt
 */
struct TunerStudioOutputChannels : output_channels_s { };

TunerStudioOutputChannels *getTunerStudioOutputChannels();
