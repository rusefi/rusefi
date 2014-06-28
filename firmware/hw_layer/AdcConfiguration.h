/**
 * @file	AdcConfiguration.h
 *
 * @date May 3, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */
#ifndef ADCCONFIGURATION_H_
#define ADCCONFIGURATION_H_

class AdcConfiguration {
public:
	AdcConfiguration(ADCConversionGroup* hwConfig);
	void addChannel(int hwChannelIndex);
	int getAdcHardwareIndexByInternalIndex(int index);
	int internalAdcIndexByHardwareIndex[20];
	int size();
	void init(void);
	int conversionCount;
private:
	ADCConversionGroup* hwConfig;
	/**
	 * Number of ADC channels in use
	 */
	int channelCount;

	int hardwareIndexByIndernalAdcIndex[20];
};


#endif /* ADCCONFIGURATION_H_ */
