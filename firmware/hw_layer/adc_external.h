/*
 * @file    adc_external.h
 *
 *
 * @date Aug 18, 2013
 * @author pc
 */

#ifndef ADC_EXTERNAL_H_
#define ADC_EXTERNAL_H_

#include "mcp3208.h"

#define getAdcValue(channel) getMcp3208adc(channel)
#define adcToVoltsDivided(adc) (5.0f / 4095 * adc)
#define getVoltageDivided(channel) (channel == EFI_ADC_NONE ? 66.66 : adcToVoltsDivided(getAdcValue(channel)))

#endif /* ADC_EXTERNAL_H_ */
