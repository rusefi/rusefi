/**
 * @file    tps.h
 * @brief
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"
#include "engine_configuration.h"

// Scaled to 1000 counts = 5.0 volts
#define TPS_TS_CONVERSION 200

bool hasPedalPositionSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE);
percent_t getPedalPosition(DECLARE_ENGINE_PARAMETER_SIGNATURE);
/**
 * Throttle Position Sensor
 * In case of dual TPS this function would return logical TPS position
 * @return Current TPS position, percent of WOT. 0 means idle and 100 means Wide Open Throttle
 */
percent_t getTPS(DECLARE_ENGINE_PARAMETER_SIGNATURE);
percent_t getTPSWithIndex(int index DECLARE_ENGINE_PARAMETER_SUFFIX);
int convertVoltageTo10bitADC(float voltage);
bool hasSecondThrottleBody(DECLARE_ENGINE_PARAMETER_SIGNATURE);
percent_t getTpsValue(int index, float adc DECLARE_ENGINE_PARAMETER_SUFFIX);
void setMockTpsAdc(percent_t tpsPosition DECLARE_ENGINE_PARAMETER_SUFFIX);
void setMockTpsValue(percent_t tpsPosition DECLARE_ENGINE_PARAMETER_SUFFIX);
void setMockThrottlePedalPosition(percent_t value DECLARE_ENGINE_PARAMETER_SUFFIX);
void grabTPSIsClosed();
void grabTPSIsWideOpen();
void grabPedalIsUp();
void grabPedalIsWideOpen();

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


