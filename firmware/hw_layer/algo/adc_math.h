/**
 * @file	adc_math.h
 *
 * @date Mar 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef ADC_MATH_H_
#define ADC_MATH_H_

// it is important that this constant is a float, not double literal
#define ADC_VCC 3.0f
#define ADC_MAX_VALUE 4095

#define adcToVolts(adc) (ADC_VCC / ADC_MAX_VALUE * (adc))

#define voltsToAdc(volts) ((volts) * (ADC_MAX_VALUE / ADC_VCC))

#define getVoltage(msg, hwChannel) (adcToVolts(getAdcValue(msg, hwChannel)))

#define getVoltageDivided(msg, hwChannel) (getVoltage(msg, hwChannel) * engineConfiguration->analogInputDividerCoefficient)

#endif /* ADC_MATH_H_ */
