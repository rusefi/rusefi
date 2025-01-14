/**
 * @file    accel_enrichment.h
 * @brief   Acceleration enrichment calculator
 *
 * @date Apr 21, 2014
 * @author Dmitry Sidin
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include <rusefi/timer.h>
#include "cyclic_buffer.h"
#include "table_helper.h"

class TpsAccelEnrichmentAutoTune : public EngineModule {
	Timer m_lastAccel;
	float m_rpm;
	float m_tpsTo;
	float m_tpsFrom;
	bool pendingUpdate = false;
	cyclic_buffer<float> cb_lambda;
	void applyUpdate();
public:
	TpsAccelEnrichmentAutoTune();
	void onNewValue();
	void onFastCallback() override;
	void onSlowCallback() override;
};

