/**
 * @file    tps.h
 * @brief
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef TPS_H_
#define TPS_H_

#include "global.h"
#include "engine_configuration.h"

// we have 12 bit precision and TS uses 10 bit precision
#define TPS_TS_CONVERSION 4

bool hasPedalPositionSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE);
percent_t getPedalPosition(DECLARE_ENGINE_PARAMETER_SIGNATURE);
/**
 * Throttle Position Sensor
 * In case of dual TPS this function would return logical TPS position
 * @return Current TPS position, percent of WOT. 0 means idle and 100 means Wide Open Throttle
 */
percent_t getTPS(DECLARE_ENGINE_PARAMETER_SIGNATURE);
bool hasTpsSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE);
int convertVoltageTo10bitADC(float voltage);
int getTPS12bitAdc(DECLARE_ENGINE_PARAMETER_SIGNATURE);
float getTPSVoltage(DECLARE_ENGINE_PARAMETER_SIGNATURE);
percent_t getTpsValue(int adc DECLARE_ENGINE_PARAMETER_SUFFIX);
void setBosch0280750009(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void setMockTpsPosition(percent_t tpsPosition);
void setMockPedalPosition(percent_t value);

typedef struct {
	efitimeus_t prevTime;
	// value 0-100%
	float prevValue;
	efitimeus_t curTime;
	// value 0-100%
	float curValue;
	// % per second
	float rateOfChange;
} tps_roc_s;

//void saveTpsState(efitimeus_t now, float curValue);
float getTpsRateOfChange(void);

#endif
