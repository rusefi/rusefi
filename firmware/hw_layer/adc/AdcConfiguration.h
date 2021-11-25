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
	adc_channel_e getAdcHardwareIndexByInternalIndex(int index) const;
	uint8_t internalAdcIndexByHardwareIndex[EFI_ADC_LAST_CHANNEL];
	bool isHwUsed(adc_channel_e hwChannel) const;
	int size() const;
	void init(void);
	uint32_t conversionCount = 0;
	uint32_t errorsCount = 0;
	int getAdcValueByIndex(int internalIndex) const;

	adcsample_t *samples;
	size_t buf_len;

	int getAdcValueByHwChannel(adc_channel_e hwChannel) const;

	adc_state values;
private:
	ADCConversionGroup* hwConfig;
	/**
	 * Number of ADC channels in use
	 */
	size_t channelCount = 0;

	/* STM32 has up-to 4 additional channels routed to internal voltage sources */
	adc_channel_e hardwareIndexByIndernalAdcIndex[ADC_MAX_CHANNELS_COUNT + 4];
};

#endif /* HAL_USE_ADC */

