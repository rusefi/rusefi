/**
 * @file	adc_inputs.h
 * @brief	Low level ADC code
 *
 * @date Jan 14, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef ADC_INPUTS_H_
#define ADC_INPUTS_H_

#include "main.h"
#include "adc_math.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

GPIO_TypeDef* getAdcChannelPort(int hwChannel);
int getAdcChannelPin(int hwChannel);
void initAdcInputs(void);
int getAdcHardwareIndexByInternalIndex(int index);
int getAdcValueByIndex(int internalIndex);
void pokeAdcInputs(void);
int getInternalAdcValue(int index);

#ifdef __cplusplus
}
#endif /* __cplusplus */


/* Depth of the conversion buffer, channels are sampled X times each.*/
#define ADC_GRP1_BUF_DEPTH_SLOW      1
#define ADC_MAX_SLOW_CHANNELS_COUNT 12

// this structure contains one multi-channel ADC state snapshot
typedef struct {
	volatile adcsample_t adc_data[ADC_MAX_SLOW_CHANNELS_COUNT];
//	time_t time;
} adc_state;

typedef struct {
	adcsample_t samples[ADC_MAX_SLOW_CHANNELS_COUNT * ADC_GRP1_BUF_DEPTH_SLOW];


} adc_hw_helper_s;


#define getAdcValue(hwChannel) getInternalAdcValue(hwChannel)

#define adcToVoltsDivided(adc) (adcToVolts(adc) * engineConfiguration->analogInputDividerCoefficient)

#endif /* ADC_INPUTS_H_ */
