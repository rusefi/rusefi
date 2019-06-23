/**
 * @file	AdcConfiguration.h
 *
 * @date May 3, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */
#ifndef ADCCONFIGURATION_H_
#define ADCCONFIGURATION_H_

#if HAL_USE_ADC

class AdcDevice {
public:
	explicit AdcDevice(ADCConversionGroup* hwConfig);
	void enableChannel(adc_channel_e hwChannelIndex);
	void enableChannelAndPin(adc_channel_e hwChannelIndex);
	adc_channel_e getAdcHardwareIndexByInternalIndex(int index) const;
	int internalAdcIndexByHardwareIndex[20];
	bool isHwUsed(adc_channel_e hwChannel) const;
	int size() const;
	void init(void);
	int conversionCount;
	int errorsCount;
	int getAdcValueByIndex(int internalIndex) const;
	void invalidateSamplesCache();

	__ALIGNED(32) adcsample_t samples[ADC_MAX_CHANNELS_COUNT * MAX_ADC_GRP_BUF_DEPTH];

	int getAdcValueByHwChannel(int hwChannel) const;

	adc_state values;
	int channelCount;
private:
	ADCConversionGroup* hwConfig;
	/**
	 * Number of ADC channels in use
	 */

	adc_channel_e hardwareIndexByIndernalAdcIndex[20];
};

#endif /* HAL_USE_ADC */

#endif /* ADCCONFIGURATION_H_ */
