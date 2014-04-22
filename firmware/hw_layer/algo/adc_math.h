/**
 * @file	adc_math.h
 *
 * @date Mar 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2013
 */

#ifndef ADC_MATH_H_
#define ADC_MATH_H_

#define ADC_VCC 3.0
#define ADC_MAX_VALUE 4095

#define adcToVolts(adc) (((ADC_VCC) * (adc) / ADC_MAX_VALUE))

#define voltsToAdc(volts) (((volts) / ADC_VCC) * ADC_MAX_VALUE)

#define getVoltage(hwChannel) (adcToVolts(getAdcValue(hwChannel)))

#define getVoltageDivided(hwChannel) (getVoltage(hwChannel) * engineConfiguration->analogInputDividerCoefficient)

#endif /* ADC_MATH_H_ */
