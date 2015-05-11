/**
 * @file    tps.h
 * @brief
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef TPS_H_
#define TPS_H_

#include "global.h"
#include "engine_configuration.h"

percent_t getTPS(DECLARE_ENGINE_PARAMETER_F);
int convertVoltageTo10bitADC(float voltage);
int getTPS10bitAdc(DECLARE_ENGINE_PARAMETER_F);
float getTPSVoltage(DECLARE_ENGINE_PARAMETER_F);
percent_t getTpsValue(int adc DECLARE_ENGINE_PARAMETER_S);

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

#endif
