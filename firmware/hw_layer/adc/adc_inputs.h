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
float boardAdjustVoltage(float voltage, adc_channel_e hwChannel);

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

int analogGetDiagnostic();

#if !defined(GPT_FREQ_FAST) || !defined(GPT_PERIOD_FAST)

/*
 * 8000 RPM is 133Hz
 * If we want to sample MAP once per 5 degrees we need 133Hz * (360 / 5) = 9576Hz of fast ADC ~= 10 KHz
 */

/*
 * TODO: migrate to continuous ADC mode? probably not - we cannot afford the callback in continuous mode.
 * TODO: look into other options
 * TODO: ADC convertion can be triggered directly from timer, with no SW intervention
 */

/* PWM clock frequency. Timer clock = 100 KHz */
#define GPT_FREQ_FAST 100000
/* PWM period (in PWM ticks), 100 KHz / 10 = 10KHz callback rate */
#define GPT_PERIOD_FAST 10

/*
 * We have 1 / (GPT_FREQ_FAST / GPT_PERIOD_FAST) to finish conversion = 100 uS
 * With ADC_SAMPLING_FAST = 28 ADC clock @ 21 MHz (F4)
 * One channel conversion takes 28 + 12 = 40 clocks
 * One channel conversion takes 1 / 21`000`000 * 40 = 1.904 uS
 * Oversampling is ADC_BUF_DEPTH_FAST = 4
 * We can do up-to 100 / (1.904 * 4) = 13.125 channels conversions
 */

#endif /* GPT_FREQ_FAST GPT_PERIOD_FAST */

#if HAL_USE_ADC

enum class AdcChannelMode : char {
	Off,
	Slow,
	Fast
};

AdcChannelMode getAdcMode(adc_channel_e hwChannel);
void initAdcInputs();

// wait until at least 1 slowADC sampling is complete
void waitForSlowAdc(uint32_t lastAdcCounter = 1);

void printFullAdcReportIfNeeded(void);
int getInternalAdcValue(const char *msg, adc_channel_e index);
float getMCUInternalTemperature(void);

void addFastAdcChannel(const char *name, adc_channel_e hwChannel);
void removeChannel(const char *name, adc_channel_e hwChannel);

#define adcGetRawValue(msg, hwChannel) getInternalAdcValue(msg, hwChannel)

#define adcRawValueToScaledVoltage(adc, hwChannel) (adcRawValueToRawVoltage(adc) * getAnalogInputDividerCoefficient(hwChannel))

// This callback is called by the ADC driver when a new fast ADC sample is ready
void onFastAdcComplete(adcsample_t* samples);

using AdcToken = uint32_t;

using AdcTockenInternal = union {
	AdcToken token;
	struct {
		uint16_t adc;
		uint16_t channel;
	} __attribute__((packed));
};

static_assert(sizeof(AdcTockenInternal) == sizeof(AdcToken));

static constexpr AdcToken invalidAdcToken = (AdcToken)(-1);

AdcToken enableFastAdcChannel(const char* msg, adc_channel_e channel);
adcsample_t getFastAdc(AdcToken token);
const ADCConversionGroup* getKnockConversionGroup(uint8_t channelIdx);
void onKnockSamplingComplete();
#endif // HAL_USE_ADC

void printFullAdcReport(void);
