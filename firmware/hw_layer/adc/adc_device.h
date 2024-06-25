/**
 * @file	adc_device.h
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

class AdcDeviceBase {
public:
	virtual int start(void) = 0;

	/* Returns internal channel number in case of success, or negative number in case of error */
	virtual int enableChannel(adc_channel_e /* hwChannel */) { return -1; }
	virtual void disableChannel(uint16_t /* token */) { }

	virtual adcsample_t get(uint16_t /* token */) { return 0; }
	virtual adcsample_t getAvg(uint16_t /* token */) { return 0; }

	virtual const char *name() { return "Dummy"; }
};

#endif /* HAL_USE_ADC */

