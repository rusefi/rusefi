/**
 * @file	AdcDevice.h
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

class AdcDevice {
public:
	explicit AdcDevice(ADCDriver *p_adcp, ADCConversionGroup* p_hwConfig, volatile adcsample_t *p_buf, size_t p_depth);
	int enableChannel(adc_channel_e hwChannel);
	/* Should be called from ISR context */
	void startConversionI(void);
	adc_channel_e getAdcChannelByInternalIndex(int index) const;
	adcsample_t getAvgAdcValue(adc_channel_e hwChannel);
	adcsample_t getAdcValueByToken(AdcToken token)
	{
		/* TODO: validate token? */

		/* TODO: in case depth > 1 this will return random (not last) sample */
		return samples[token];
	};
	AdcToken getAdcChannelToken(adc_channel_e hwChannel);
	int size() const;
	void init(void);

private:
	ADCDriver *adcp;
	ADCConversionGroup* hwConfig;
	volatile adcsample_t *samples;
	size_t depth;
	uint8_t internalAdcIndexByHardwareIndex[EFI_ADC_TOTAL_CHANNELS];
	/**
	 * Number of ADC channels in use
	 */
	size_t channelCount = 0;
};

#endif /* HAL_USE_ADC */

