/**
 * @file	adc_inputs.h
 * @brief	Low level internal ADC code
 *
 * @date Jan 14, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"
#include "adc_math.h"

#ifndef SLOW_ADC_RATE
#define SLOW_ADC_RATE 500
#endif

float getAnalogInputDividerCoefficient(adc_channel_e);

inline bool isAdcChannelValid(adc_channel_e hwChannel) {
	/* Compiler will optimize, keep following if as a reminder */
	if (hwChannel >= EFI_ADC_TOTAL_CHANNELS) {
		/* this should not happen!
		 * if we have enum out of range somewhere in settings
		 * that means something goes terribly wrong
		 * TODO: should we say something?
		 */
		return false;
	}
	return ((hwChannel > EFI_ADC_NONE) && (hwChannel < EFI_ADC_TOTAL_CHANNELS));
}

inline bool isAdcChannelOnChip(adc_channel_e hwChannel) {
	return (isAdcChannelValid(hwChannel) && (hwChannel <= EFI_ADC_ONCHIP_LAST));
}

inline bool isAdcChannelOffChip(adc_channel_e hwChannel) {
	return (isAdcChannelValid(hwChannel) && (hwChannel > EFI_ADC_ONCHIP_LAST));
}


#if !defined(GPT_FREQ_FAST) || !defined(GPT_PERIOD_FAST)
/**
 * 8000 RPM is 133Hz
 * If we want to sample MAP once per 5 degrees we need 133Hz * (360 / 5) = 9576Hz of fast ADC
 */
// todo: migrate to continuous ADC mode? probably not - we cannot afford the callback in
// todo: continuous mode. todo: look into our options
#define GPT_FREQ_FAST 100000   /* PWM clock frequency. I wonder what does this setting mean?  */
#define GPT_PERIOD_FAST 10  /* PWM period (in PWM ticks).    */
#endif /* GPT_FREQ_FAST GPT_PERIOD_FAST */

#if HAL_USE_ADC

typedef enum {
	ADC_OFF = 0,
	ADC_SLOW = 1,
	ADC_FAST = 2,
} adc_channel_mode_e;

adc_channel_mode_e getAdcMode(adc_channel_e hwChannel);
void initAdcInputs();

// wait until at least 1 slowADC sampling is complete
void waitForSlowAdc(uint32_t lastAdcCounter = 0);

void printFullAdcReportIfNeeded(void);
int getInternalAdcValue(const char *msg, adc_channel_e index);
float getMCUInternalTemperature(void);

void addChannel(const char *name, adc_channel_e hwChannel, adc_channel_mode_e mode);
void removeChannel(const char *name, adc_channel_e hwChannel);

#define getAdcValue(msg, hwChannel) getInternalAdcValue(msg, hwChannel)

#define adcToVoltsDivided(adc, hwChannel) (adcToVolts(adc) * getAnalogInputDividerCoefficient(hwChannel))

// This callback is called by the ADC driver when a new fast ADC sample is ready
void onFastAdcComplete(adcsample_t* samples);

using FastAdcToken = size_t;
static constexpr FastAdcToken invalidAdcToken = (FastAdcToken)(-1);

FastAdcToken enableFastAdcChannel(const char* msg, adc_channel_e channel);
adcsample_t getFastAdc(FastAdcToken token);
#endif // HAL_USE_ADC

void printFullAdcReport(void);
