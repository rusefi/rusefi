/**
 * @file    accel_enrichment.h
 * @brief   Acceleration enrichment calculator
 *
 * @date Apr 21, 2014
 * @author Dmitry Sidin
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "cyclic_buffer.h"
#include "table_helper.h"
#include "wall_fuel_state_generated.h"

typedef Map3D<TPS_TPS_ACCEL_TABLE, TPS_TPS_ACCEL_TABLE, float, float> tps_tps_Map3D_t;

/**
 * this object is used for MAP rate-of-change and TPS rate-of-change corrections
 */
class AccelEnrichment {
public:
	AccelEnrichment();
	int getMaxDeltaIndex(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	float getMaxDelta(DECLARE_ENGINE_PARAMETER_SIGNATURE);

	void resetAE();
	void setLength(int length);
	cyclic_buffer<float> cb;
	void onNewValue(float currentValue DECLARE_ENGINE_PARAMETER_SUFFIX);
	int onUpdateInvocationCounter = 0;
};

class TpsAccelEnrichment : public AccelEnrichment {
public:
	/**
	 * @return Extra fuel squirt duration for TPS acceleration
	 */
	floatms_t getTpsEnrichment(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	void onEngineCycleTps(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	void resetFractionValues();
	void resetAE();
private:
	/**
	 * Used for Fractional TPS enrichment. 
	 */
	floatms_t accumulatedValue = 0;
	floatms_t maxExtraPerCycle = 0;
	floatms_t maxExtraPerPeriod = 0;
	floatms_t maxInjectedPerPeriod = 0;
	int cycleCnt = 0;
};

void initAccelEnrichment(DECLARE_ENGINE_PARAMETER_SIGNATURE);

void setTpsAccelThr(float value);
void setTpsDecelThr(float value);
void setTpsDecelMult(float value);
void setTpsAccelLen(int length);

void updateAccelParameters();
