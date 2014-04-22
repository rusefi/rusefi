/**
 * @file	map_averaging.h
 *
 * @date Dec 11, 2013
 * @author Andrey Belomutskiy, (c) 2012-2013
 */

#ifndef ADC_AVERAGING_H_
#define ADC_AVERAGING_H_

void mapAveragingCallback(adcsample_t newValue);
void initMapAveraging(void);
float getAtmosphericPressure(void);

#endif /* ADC_AVERAGING_H_ */
