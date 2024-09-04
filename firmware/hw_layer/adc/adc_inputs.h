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
 * With ADC_SAMPLING_FAST = 28 ADC clock @ 21 MHz (F4) -> one channel conversion takes 1.33(3) uS
 * Oversampling is ADC_BUF_DEPTH_FAST = 4
 * We can do up-to 100 / (1.33(3) * 4) = 18.75 channels conversions
 * So we can enable ALL channels for fast ADC.
 * This will increase bus load with more DMA transfers, but who cares?
 */

#endif /* GPT_FREQ_FAST GPT_PERIOD_FAST */

#if HAL_USE_ADC

typedef enum {
	ADC_SLOW = 0,
	ADC_FAST = 1,
	ADC_AUX = 2,

	ADC_EXTERNAL_0 = 8,
	ADC_EXTERNAL_1 = 9,
	ADC_EXTERNAL_3 = 10,

	ADC_OFF = 16,
} adc_channel_mode_e;

#define ADC_DRIVERS_COUNT	ADC_OFF

using AdcToken = uint32_t;

using AdcTockenInternal = union {
	AdcToken token;
	struct {
		uint16_t drvIdx;
		uint16_t channel;
	} __attribute__((packed));
};

static constexpr AdcToken invalidAdcToken = (AdcToken)(0xffffffff);

adc_channel_mode_e getAdcMode(adc_channel_e hwChannel);
void initAdcInputs();

// wait until at least 1 slowADC sampling is complete
void waitForSlowAdc(uint32_t lastAdcCounter = 0);

void printFullAdcReportIfNeeded(void);
int getInternalAdcValue(const char *msg, adc_channel_e index);
float getMCUInternalTemperature(void);

AdcToken addChannel(const char *name, adc_channel_e hwChannel, adc_channel_mode_e mode);
AdcToken addFastAdcChannel(const char *msg, adc_channel_e hwChannel);
void removeChannel(const char *name, adc_channel_e hwChannel);

#define getAdcValue(msg, hwChannel) getInternalAdcValue(msg, hwChannel)

#define adcToVoltsDivided(adc, hwChannel) (adcToVolts(adc) * getAnalogInputDividerCoefficient(hwChannel))

// This callback is called by the ADC driver when a new fast ADC sample is ready
void onFastAdcComplete(adcsample_t* samples);

static_assert(sizeof(AdcTockenInternal) == sizeof(AdcToken));

AdcToken enableFastAdcChannel(const char* msg, adc_channel_e channel);
adcsample_t getFastAdc(AdcToken token);
#endif // HAL_USE_ADC

void printFullAdcReport(void);
