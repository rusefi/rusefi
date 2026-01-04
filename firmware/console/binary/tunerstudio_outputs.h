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

/**
 * see also [OutputChannels] in tunerstudio.template.ini
 */
 struct TunerStudioOutputChannels : output_channels_s { };

TunerStudioOutputChannels *getTunerStudioOutputChannels();