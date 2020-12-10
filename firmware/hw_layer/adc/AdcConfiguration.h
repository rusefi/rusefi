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
	explicit AdcDevice(ADCConversionGroup* hwConfig, adcsample_t *buf, size_t buf_len);
	void enableChannel(adc_channel_e hwChannelIndex);
	void enableChannelAndPin(const char *msg, adc_channel_e hwChannelIndex);
	adc_channel_e getAdcHardwareIndexByInternalIndex(int index) const;
	int internalAdcIndexByHardwareIndex[ADC_MAX_CHANNELS_COUNT + 4];
	bool isHwUsed(adc_channel_e hwChannel) const;
	int size() const;
	void init(void);
	uint32_t conversionCount = 0;
	uint32_t errorsCount = 0;
	int getAdcValueByIndex(int internalIndex) const;
	void invalidateSamplesCache();

	adcsample_t *samples;
	size_t buf_len;

	int getAdcValueByHwChannel(adc_channel_e hwChannel) const;

	adc_state values;
	size_t channelCount = 0;
private:
	ADCConversionGroup* hwConfig;
	/**
	 * Number of ADC channels in use
	 */
	 
	adc_channel_e hardwareIndexByIndernalAdcIndex[ADC_MAX_CHANNELS_COUNT + 4];
};

#endif /* HAL_USE_ADC */

