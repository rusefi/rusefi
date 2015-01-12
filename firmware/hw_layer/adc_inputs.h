/**
 * @file	adc_inputs.h
 * @brief	Low level ADC code
 *
 * @date Jan 14, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef ADC_INPUTS_H_
#define ADC_INPUTS_H_

#include "main.h"
#include "adc_math.h"

#if HAL_USE_ADC || defined(__DOXYGEN__)


const char * getAdcMode(adc_channel_e hwChannel);
int getAdcChannelPin(adc_channel_e hwChannel);
void initAdcInputs(bool boardTestMode);
GPIO_TypeDef* getAdcChannelPort(adc_channel_e hwChannel);
adc_channel_e getAdcChannel(brain_pin_e pin);

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

int getAdcHardwareIndexByInternalIndex(int index);
void pokeAdcInputs(void);
int getInternalAdcValue(adc_channel_e index);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/* Depth of the conversion buffer, channels are sampled X times each.*/
#define ADC_GRP1_BUF_DEPTH_SLOW      1
#define ADC_MAX_CHANNELS_COUNT 16

//typedef struct

// this structure contains one multi-channel ADC state snapshot
typedef struct {
	volatile adcsample_t adc_data[ADC_MAX_CHANNELS_COUNT];
//	time_t time;
} adc_state;

typedef struct {
	adcsample_t samples[ADC_MAX_CHANNELS_COUNT * ADC_GRP1_BUF_DEPTH_SLOW];


} adc_hw_helper_s;


#define getAdcValue(hwChannel) getInternalAdcValue(hwChannel)

// todo: migrate to adcToVoltageInputDividerCoefficient
#define adcToVoltsDivided(adc) (adcToVolts(adc) * engineConfiguration->analogInputDividerCoefficient)

#else
#define getAdcValue(channel) 0
#endif /* HAL_USE_ADC */

#endif /* ADC_INPUTS_H_ */
