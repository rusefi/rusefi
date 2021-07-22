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

#define SLOW_ADC_RATE 500

static inline bool isAdcChannelValid(adc_channel_e hwChannel) {
	if (hwChannel <= EFI_ADC_NONE) {
		return false;
	} else if (hwChannel >= EFI_ADC_LAST_CHANNEL) {
		/* this should not happen!
		 * if we have enum out of range somewhere in settings
		 * that means something goes terribly wrong
		 * TODO: should we say something?
		 */
		return false;
	} else {
		return true;
	}
}

#if HAL_USE_ADC

adc_channel_mode_e getAdcMode(adc_channel_e hwChannel);
void initAdcInputs();

// deprecated - migrate to 'getAdcChannelBrainPin'
int getAdcChannelPin(adc_channel_e hwChannel);

// deprecated - migrate to 'getAdcChannelBrainPin'
ioportid_t getAdcChannelPort(const char *msg, adc_channel_e hwChannel);

adc_channel_e getAdcChannel(brain_pin_e pin);
brain_pin_e getAdcChannelBrainPin(const char *msg, adc_channel_e hwChannel);

// wait until at least 1 slowADC sampling is complete
void waitForSlowAdc(uint32_t lastAdcCounter = 0);
// get a number of completed slowADC samples
int getSlowAdcCounter();

int getAdcHardwareIndexByInternalIndex(int index);

void printFullAdcReportIfNeeded(void);
int getInternalAdcValue(const char *msg, adc_channel_e index);
float getMCUInternalTemperature(void);

void addChannel(const char *name, adc_channel_e setting, adc_channel_mode_e mode);
void removeChannel(const char *name, adc_channel_e setting);

#define getAdcValue(msg, hwChannel) getInternalAdcValue(msg, hwChannel)

#define adcToVoltsDivided(adc) (adcToVolts(adc) * engineConfiguration->analogInputDividerCoefficient)

#else
#define getAdcValue(msg, channel) 0
#endif /* HAL_USE_ADC */

void printFullAdcReport(void);

// This callback is called by the ADC driver when a new fast ADC sample is ready
void onFastAdcComplete(adcsample_t* samples);
