/**
 * @file    accel_enrichment.h
 * @brief   Acceleration enrichment calculator
 *
 * @date Apr 21, 2014
 * @author Dmitry Sidin
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef ACC_ENRICHMENT_H_
#define ACC_ENRICHMENT_H_

#include "global.h"
#include "cyclic_buffer.h"
#include "table_helper.h"

typedef Map3D<TPS_TPS_ACCEL_TABLE, TPS_TPS_ACCEL_TABLE, float> tps_tps_Map3D_t;

/**
 * this object is used for MAP rate-of-change and TPS rate-of-change corrections
 */
class AccelEnrichmemnt {
public:
	AccelEnrichmemnt();
	/**
	 * @return Extra engine load value for fuel logic calculation
	 */
	float getEngineLoadEnrichment(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	/**
	 * @return Extra fuel squirt duration for TPS acceleration
	 */
	floatms_t getTpsEnrichment(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	int getMaxDeltaIndex(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	float getMaxDelta(DECLARE_ENGINE_PARAMETER_SIGNATURE);

	void onEngineCycle(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	void onEngineCycleTps(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	void reset();
	void resetFractionValues();
	void setLength(int length);
	cyclic_buffer<float> cb;
	void onNewValue(float currentValue DECLARE_ENGINE_PARAMETER_SUFFIX);

private:
	float previousValue;
	/**
	 * Used for Fractional TPS enrichment. 
	 */
	floatms_t accumulatedValue;
	floatms_t maxExtraPerCycle;
	floatms_t maxExtraPerPeriod;
	floatms_t maxInjectedPerPeriod;
	int cycleCnt;
};

/**
 * Wall wetting, also known as fuel film
 * See https://github.com/rusefi/rusefi/issues/151 for the theory
 */
class WallFuel {
public:
	WallFuel();
	/**
	 * @param target desired squirt duration
	 * @return total adjusted fuel squirt duration once wall wetting is taken into effect
	 */
	floatms_t adjust(int injectorIndex, floatms_t target DECLARE_ENGINE_PARAMETER_SUFFIX);
	floatms_t getWallFuel(int injectorIndex);
	void reset();
private:
	/**
	 * Amount of fuel on the wall, in ms of injector open time, for specific injector.
	 */
	floatms_t wallFuel[INJECTION_PIN_COUNT];
};

void initAccelEnrichment(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX);

void setEngineLoadAccelLen(int len);
void setEngineLoadAccelThr(float value);
void setEngineLoadAccelMult(float value);

void setTpsAccelThr(float value);
void setTpsDecelThr(float value);
void setTpsDecelMult(float value);
void setTpsAccelLen(int length);

void setDecelThr(float value);
void setDecelMult(float value);

void updateAccelParameters();

#endif /* ACC_ENRICHMENT_H_ */

