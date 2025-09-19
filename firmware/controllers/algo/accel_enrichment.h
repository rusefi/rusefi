/**
 * @file    accel_enrichment.h
 * @brief   Acceleration enrichment calculator
 *
 * @date Apr 21, 2014
 * @author Dmitry Sidin
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "efitime.h"
#include "cyclic_buffer.h"
#include "table_helper.h"
#include "wall_fuel_state_generated.h"
#include "tps_accel_state_generated.h"


class TpsAccelEnrichment : public tps_accel_state_s, public EngineModule {
#ifdef EFI_UNIT_TEST
public:
#endif
	// This flag is set by onNewValue() when an accel event is detected
	// and cleared by isAccelEventTriggered() after being read.
	bool m_accelEventJustOccurred = false;
public:
	TpsAccelEnrichment();

	void onConfigurationChange(engine_configuration_s const* previousConfig) override;

	Timer m_timeSinceAccel;
	// This function returns true ONCE per acceleration event.
	bool isAccelEventTriggered();

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
	float getTimeSinceAcell() const;
};

void initAccelEnrichment();
