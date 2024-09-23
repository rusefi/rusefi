/**
 * @file	adc_onchip.h
 *
 * @date May 3, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "adc_device.h"

#if HAL_USE_ADC

class AdcSlowOnchipDevice : public AdcDeviceBase {
public:
	int start(void) override;
	const char *name() override { return "SlowAdc"; }
	bool isChannelSupported(adc_channel_e hwChannel) override;
	int enableChannel(adc_channel_e hwChannel) override;
	void disableChannel(uint16_t token) override;

	explicit AdcSlowOnchipDevice(ADCDriver *p_adcp, volatile adcsample_t *p_buf);
	/* blocks until convertion is done */
	void doConversion(void);

	adcsample_t get(uint16_t token) override;
	adcsample_t getAvg(uint16_t token) override;

	int size() const;
	uint32_t conversionCount = 0;

private:
	ADCDriver *adcp;
	volatile adcsample_t *samples;
	/**
	 * Number of ADC channels in use
	 */
	size_t channelCount = 0;
};

class AdcFastOnchipDevice : public AdcDeviceBase {
public:
	int start(void) override;
	const char *name() override { return "FastAdc"; }
	bool isChannelSupported(adc_channel_e hwChannel) override;
	int enableChannel(adc_channel_e hwChannel) override;
	void disableChannel(uint16_t token) override;

	explicit AdcFastOnchipDevice(ADCDriver *p_adcp, volatile adcsample_t *p_buf, size_t p_depth);
	/* Should be called from ISR context */
	void startConversionI(void);

	adcsample_t get(uint16_t token) override;
	adcsample_t getAvg(uint16_t token) override;

	int size() const;
	uint32_t conversionCount = 0;

private:
	ADCDriver *adcp;
	ADCConversionGroup hwConfig;
	volatile adcsample_t *samples;
	size_t depth;
	/**
	 * Number of ADC channels in use
	 */
	size_t channelCount = 0;
};

#endif /* HAL_USE_ADC */
