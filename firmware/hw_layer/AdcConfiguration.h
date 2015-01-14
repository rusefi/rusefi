/**
 * @file	AdcConfiguration.h
 *
 * @date May 3, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */
#ifndef ADCCONFIGURATION_H_
#define ADCCONFIGURATION_H_

#if HAL_USE_ADC || defined(__DOXYGEN__)

class AdcDevice {
public:
	AdcDevice(ADCConversionGroup* hwConfig);
	void addChannel(adc_channel_e hwChannelIndex);
	adc_channel_e getAdcHardwareIndexByInternalIndex(int index);
	int internalAdcIndexByHardwareIndex[20];
	bool isHwUsed(adc_channel_e hwChannel);
	int size();
	void init(void);
	int conversionCount;
	int errorsCount;
	int getAdcValueByIndex(int internalIndex);

	adcsample_t samples[ADC_MAX_CHANNELS_COUNT * ADC_GRP1_BUF_DEPTH_SLOW];

	adc_state values;
private:
	ADCConversionGroup* hwConfig;
	/**
	 * Number of ADC channels in use
	 */
	int channelCount;

	adc_channel_e hardwareIndexByIndernalAdcIndex[20];
};

#endif /* HAL_USE_ADC */

#endif /* ADCCONFIGURATION_H_ */
