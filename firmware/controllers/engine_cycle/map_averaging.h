/**
 * @file	map_averaging.h
 *
 * @date Dec 11, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#if EFI_MAP_AVERAGING

#if HAL_USE_ADC
void mapAveragingAdcCallback(adcsample_t newValue);
#endif

void initMapAveraging();
void refreshMapAveragingPreCalc();

void mapAveragingTriggerCallback(
		uint32_t index, efitick_t edgeTimestamp);

void onMapAveraged(float mapKpa, efitick_t nowNt);
SensorResult convertMap(float volts);

#if EFI_TUNER_STUDIO
void postMapState(TunerStudioOutputChannels *tsOutputChannels);
#endif

#endif /* EFI_MAP_AVERAGING */
