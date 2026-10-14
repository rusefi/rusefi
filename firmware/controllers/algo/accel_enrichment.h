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
#if EFI_UNIT_TEST
public:
#endif
	// This flag is set by onNewValue() when an accel event is detected
	// and cleared by isAccelEventTriggered() after being read.
	bool m_accelEventJustOccurred = false;
	mass_t m_luaEnrichment = 0;
	Timer m_luaEnrichmentTimer;
public:
	TpsAccelEnrichment();

	void onConfigurationChange(engine_configuration_s const* previousConfig) override;
	void onEngineStop() override;

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

	// [tag:lua_accel_enrichment] Lua supplies extra grams per cylinder per engine
	// cycle. The fuel pipeline applies injection-mode scaling exactly once.
	// Only AE_MODE_LUA consumes it, with a 200 ms timeout. Reset on Lua reload,
	// engine stop and mode changes so a previous script/event cannot linger.
	bool setLuaEnrichment(mass_t grams);
	mass_t getLuaEnrichment();
	void resetLua();
};

void initAccelEnrichment();
