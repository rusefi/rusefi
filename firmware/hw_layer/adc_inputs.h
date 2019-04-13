/**
 * @file	adc_inputs.h
 * @brief	Low level internal ADC code
 *
 * @date Jan 14, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef ADC_INPUTS_H_
#define ADC_INPUTS_H_

#include "global.h"
#include "adc_math.h"

#if HAL_USE_ADC

const char * getAdcMode(adc_channel_e hwChannel);
void initAdcInputs(bool boardTestMode);
void adc_callback_fast(ADCDriver *adcp, adcsample_t *buffer, size_t n);

// deprecated - migrate to 'getAdcChannelBrainPin'
int getAdcChannelPin(adc_channel_e hwChannel);

// deprecated - migrate to 'getAdcChannelBrainPin'
ioportid_t getAdcChannelPort(const char *msg, adc_channel_e hwChannel);

adc_channel_e getAdcChannel(brain_pin_e pin);
brain_pin_e getAdcChannelBrainPin(const char *msg, adc_channel_e hwChannel);

// wait until at least 1 slowADC sampling is complete
void waitForSlowAdc(int lastAdcCounter = 0);
// get a number of completed slowADC samples
int getSlowAdcCounter();

int getAdcHardwareIndexByInternalIndex(int index);

void printFullAdcReportIfNeeded(Logging *log);
int getInternalAdcValue(const char *msg, adc_channel_e index);
float getMCUInternalTemperature(void);

void addChannel(const char *name, adc_channel_e setting, adc_channel_mode_e mode);
void removeChannel(const char *name, adc_channel_e setting);

// max(ADC_BUF_DEPTH_SLOW, ADC_BUF_DEPTH_FAST)
#define MAX_ADC_GRP_BUF_DEPTH 8

#define ADC_MAX_CHANNELS_COUNT 16

//typedef struct

// this structure contains one multi-channel ADC state snapshot
typedef struct {
	volatile adcsample_t adc_data[ADC_MAX_CHANNELS_COUNT];
//	time_t time;
} adc_state;

#define getAdcValue(msg, hwChannel) getInternalAdcValue(msg, hwChannel)

// todo: migrate to adcToVoltageInputDividerCoefficient
#define adcToVoltsDivided(adc) (adcToVolts(adc) * engineConfiguration->analogInputDividerCoefficient)

#else
#define getAdcValue(msg, channel) 0
#endif /* HAL_USE_ADC */

#endif /* ADC_INPUTS_H_ */
