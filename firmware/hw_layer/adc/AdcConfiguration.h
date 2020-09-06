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
	explicit AdcDevice(ADCDriver *adcp, ADCConversionGroup* hwConfig, adc_channel_mode_e adc_mode, adcsample_t *buf, int buf_size);

	int enableChannel(adc_channel_e hwChannelIndex);
	int enableChannelAndPin(const char *msg, adc_channel_e hwChannelIndex);

	int doConvertion(void);
	int startConvertion(void);

	adcsample_t getAvgAdcValueByIndex(int index);
	/* TODO: need rework */
	adcsample_t getAdcValueByIndex(int internalIndex) const;

	adc_channel_e getAdcHardwareIndexByInternalIndex(int index) const;
	int internalAdcIndexByHardwareIndex[ADC_MAX_CHANNELS_COUNT + 4];
	bool isHwUsed(adc_channel_e hwChannel) const;
	int size() const;
	void init(void);
	int conversionCount;
	int errorsCount;
	void invalidateSamplesCache();
	void cleanSamplesCache();

	adcsample_t *samples;
	int buf_depth;

	int getAdcValueByHwChannel(int hwChannel) const;

	adc_state values;
private:
	adc_channel_mode_e mode;
	ADCDriver *adcp;
	ADCConversionGroup* hwConfig;
	/**
	 * Number of ADC channels in use
	 */
	int channelCount;
	/* mapping */
	adc_channel_e hardwareIndexByIndernalAdcIndex[ADC_MAX_CHANNELS_COUNT + 4];
};

#endif /* HAL_USE_ADC */

