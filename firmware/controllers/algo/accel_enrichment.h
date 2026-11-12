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

	// [tag:tps_rate_detector] Up to two previous TPS samples: the rate is measured
	// against the oldest, about 10 ms back at the 200 Hz sampling rate.
	struct TpsSample {
		float tps;
		efitick_t nt;
	};
	TpsSample m_tpsHistory[2] = {};
	int m_tpsHistoryCount = 0;
	Timer m_accelHoldTimer;
	Timer m_decelHoldTimer;

	// [tag:tps_rate_detector] Raw TPS samples for the median of tpsEventConfirmation:
	// with N samples the detector uses the median of the last 2N-1, so a change must
	// last N samples to be seen.
	static constexpr int TPS_MEDIAN_MAX = 5;
	float m_rawTps[TPS_MEDIAN_MAX] = {};
	int m_rawTpsCount = 0;
	efitick_t m_lastRawNt = 0;

	void clearDetector();
public:
	TpsAccelEnrichment();

	void onConfigurationChange(engine_configuration_s const* previousConfig) override;
	void onEngineStop() override;

	Timer m_timeSinceAccel;
	// This function returns true ONCE per acceleration event.
	bool isAccelEventTriggered();

	/**
	 * [tag:tps_rate_detector] Feeds one TPS sample (percent) taken at nowNt.
	 * Called from Engine::periodicFastCallback() before the fuel math. Samples
	 * closer than TPS_RATE_MIN_SAMPLE_US to the previous one are ignored, so extra
	 * fast-callback invocations do not shorten the rate window.
	 */
	void onNewValue(float currentValue, efitick_t nowNt);
	// Invalid TPS: drop history and end any event, no rate is reported.
	void onInvalidTps();
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
