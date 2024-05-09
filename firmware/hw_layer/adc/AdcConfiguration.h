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

#ifndef SLOW_ADC_CHANNEL_COUNT
#ifdef ADC_MUX_PIN
#define SLOW_ADC_CHANNEL_COUNT 32
#else // not ADC_MUX_PIN
#define SLOW_ADC_CHANNEL_COUNT 16
#endif // def ADC_MUX_PIN
#endif // SLOW_ADC_CHANNEL_COUNT

// this structure contains one multi-channel ADC state snapshot
typedef struct {
	volatile adcsample_t adc_data[ADC_MAX_CHANNELS_COUNT];
} adc_state;

class AdcDevice {
public:
	explicit AdcDevice(ADCConversionGroup* p_hwConfig, adcsample_t *p_buf);
	void enableChannel(adc_channel_e hwChannelIndex);
	adc_channel_e getAdcChannelByInternalIndex(int index) const;
	uint8_t internalAdcIndexByHardwareIndex[EFI_ADC_TOTAL_CHANNELS];
	int size() const;
	void init(void);
	uint32_t conversionCount = 0;

	adcsample_t *samples;
private:
	ADCConversionGroup* hwConfig;
	/**
	 * Number of ADC channels in use
	 */
	size_t channelCount = 0;
};

#endif /* HAL_USE_ADC */

