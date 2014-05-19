/**
 * @file	map_averaging.h
 *
 * @date Dec 11, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef ADC_AVERAGING_H_
#define ADC_AVERAGING_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void mapAveragingCallback(adcsample_t newValue);
void initMapAveraging(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ADC_AVERAGING_H_ */
