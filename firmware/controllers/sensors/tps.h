/**
 * @file    tps.h
 * @brief
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef TPS_H_
#define TPS_H_

#include "global.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

float getTPS(void);
int convertVoltageTo10bitADC(float voltage);
int getTPS10bitAdc(void);
float getTPSVoltage(void);

typedef struct {
	// time in systicks
	// todo: one day we should migrate all times to float seconds or milliseconds?
	time_t prevTime;
	// value 0-100%
	float prevValue;
	// time in systicks
	time_t curTime;
	// value 0-100%
	float curValue;
	// % per second
	float rateOfChange;
} tps_roc_s;

void saveTpsState(time_t now, float curValue);
float getTpsRateOfChange(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
