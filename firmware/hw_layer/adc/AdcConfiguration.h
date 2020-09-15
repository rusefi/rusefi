/**
 * @file	AdcConfiguration.h
 *
 * @date May 3, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#if HAL_USE_ADC

#ifndef ADC_MAX_CHANNELS_COUNT
#define ADC_MAX_CHANNELS_COUNT 16
#endif /* ADC_MAX_CHANNELS_COUNT */

// this structure contains one multi-channel ADC state snapshot
typedef struct {
	volatile adcsample_t adc_data[ADC_MAX_CHANNELS_COUNT];
} adc_state;

class AdcDevice {
public:
	explicit AdcDevice(ADCConversionGroup* hwConfig, adcsample_t *buf);
	void enableChannel(adc_channel_e hwChannelIndex);
	void enableChannelAndPin(const char *msg, adc_channel_e hwChannelIndex);
	adc_channel_e getAdcHardwareIndexByInternalIndex(int index) const;
	int internalAdcIndexByHardwareIndex[ADC_MAX_CHANNELS_COUNT + 4];
	bool isHwUsed(adc_channel_e hwChannel) const;
	int size() const;
	void init(void);
	int conversionCount;
	int errorsCount;
	int getAdcValueByIndex(int internalIndex) const;
	void invalidateSamplesCache();

	adcsample_t *samples;

	int getAdcValueByHwChannel(int hwChannel) const;

	adc_state values;
	int channelCount;
private:
	ADCConversionGroup* hwConfig;
	/**
	 * Number of ADC channels in use
	 */
	 
	adc_channel_e hardwareIndexByIndernalAdcIndex[ADC_MAX_CHANNELS_COUNT + 4];
};

#endif /* HAL_USE_ADC */

