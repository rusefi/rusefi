/**
 * @file	analog_chart.h
 *
 * @date Dec 20, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef ANALOG_CHART_H_
#define ANALOG_CHART_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void acAddData(float angle, float value);
void initAnalogChart(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ANALOG_CHART_H_ */
