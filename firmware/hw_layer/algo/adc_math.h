/**
 * @file	adc_math.h
 *
 * todo: all this looks to be about internal ADC, merge this file with
 *
 * @date Mar 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef ADC_MATH_H_
#define ADC_MATH_H_

#define ADC_MAX_VALUE 4095

#define adcToVolts(adc) ((engineConfiguration->adcVcc) / ADC_MAX_VALUE * (adc))

#define voltsToAdc(volts) ((volts) * (ADC_MAX_VALUE / (engineConfiguration->adcVcc)))

#define getVoltage(msg, hwChannel) (adcToVolts(getAdcValue(msg, hwChannel)))

#define getVoltageDivided(msg, hwChannel) (getVoltage(msg, hwChannel) * engineConfiguration->analogInputDividerCoefficient)

#endif /* ADC_MATH_H_ */
