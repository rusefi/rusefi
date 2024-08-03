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
#include "tps_accel_state_generated.h"

typedef Map3D<TPS_TPS_ACCEL_TABLE, TPS_TPS_ACCEL_TABLE, float, float, float> tps_tps_Map3D_t;

class TpsAccelEnrichment : public tps_accel_state_s, public EngineModule {
public:
	TpsAccelEnrichment();

	void onConfigurationChange(engine_configuration_s const* previousConfig) override;

	int getMaxDeltaIndex();
	float getMaxDelta();

	void setLength(int length);
	cyclic_buffer<float> cb;
	void onNewValue(float currentValue);
	int onUpdateInvocationCounter = 0;

	/**
	 * @return Extra fuel squirt duration for TPS acceleration
	 */
	floatms_t getTpsEnrichment();
	void onEngineCycleTps();
	void resetFractionValues();
	void resetAE();
};

void initAccelEnrichment();
