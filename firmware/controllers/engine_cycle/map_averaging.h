/**
 * @file	map_averaging.h
 *
 * @date Dec 11, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine.h"

#if EFI_MAP_AVERAGING

#if HAL_USE_ADC
void mapAveragingAdcCallback(adcsample_t newValue);
#endif

void initMapAveraging(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX);
void refreshMapAveragingPreCalc(DECLARE_ENGINE_PARAMETER_SIGNATURE);

#if EFI_TUNER_STUDIO
void postMapState(TunerStudioOutputChannels *tsOutputChannels);
#endif

#endif /* EFI_MAP_AVERAGING */
