/**
 * @file    accel_enrichment.cpp
 * @brief   Acceleration enrichment calculator
 *
 * accelEnrichmentMode selects one of four acceleration enrichment strategies:
 *
 * 1) MS Adder: TPS-based extra injection duration, converted to fuel mass.
 * 2) Percent Adder: TPS-based extra fuel proportional to injection mass.
 * 3) MAP Prediction: TPS-triggered MAP prediction in speed_density_airmass.cpp.
 * 4) Lua: script-supplied extra cycle fuel mass, with a freshness timeout.
 *
 * This file implements the shared TPS detector and the two adders.
 * Wall wetting in wall_fuel.cpp is independent and can operate with any mode.
 *
 * http://rusefi.com/wiki/index.php?title=Manual:Software:Fuel_Control
 * @date Apr 21, 2014
 * @author Dmitry Sidin
 * @author Andrey Belomutskiy, (c) 2012-2020
 * @author Matthew Kennedy
 */

#include "pch.h"
#include "accel_enrichment.h"
#include "flex_transient.h"
#include "tunerstudio.h"


// on this level we do not distinguish between multiplier and 'ms adder' modes
float TpsAccelEnrichment::getTpsEnrichment() {
	ScopePerf perf(PE::GetTpsEnrichment);

	// Lua and predictive MAP replace the legacy TPS adders.
	if (engineConfiguration->accelEnrichmentMode == AE_MODE_PREDICTIVE_MAP
		|| engineConfiguration->accelEnrichmentMode == AE_MODE_LUA) {
		return 0;
	}

	if (engineConfiguration->tpsAccelHoldTime <= 0) {
		// Zero hold time disables the additive modes.
		return 0;
	}

#if EFI_TUNER_STUDIO
	if (isTuningVeNow()) {
		return 0;
	}
#endif

	float rpm = Sensor::getOrZero(SensorType::Rpm);
	if (rpm < engineConfiguration->cranking.rpm) {
		return 0;
	}

	if (isAboveAccelThreshold) {
		valueFromTable = interpolate3d(config->tpsTpsAccelTable,
			config->tpsTpsAccelToRpmBins, tpsTo,
			config->tpsTpsAccelFromRpmBins, tpsFrom);
		extraFuel = valueFromTable;
		m_timeSinceAccel.reset();
	} else if (isBelowDecelThreshold) {
		extraFuel = deltaTps * engineConfiguration->tpsDecelEnleanmentMultiplier;
		m_timeSinceAccel.reset();
	} else {
		extraFuel = 0;
	}

	// Fractional enrichment (fuel portions are accumulated and split between several engine cycles.
	// This is a crude imitation of carburetor's acceleration pump.
	isFractionalEnrichment = engineConfiguration->tpsAccelFractionPeriod > 1 || engineConfiguration->tpsAccelFractionDivisor > 1.0f;
	if (isFractionalEnrichment) {
		// make sure both values are non-zero
		float periodF = std::max<int>(engineConfiguration->tpsAccelFractionPeriod, 1);
		float divisor = std::max(engineConfiguration->tpsAccelFractionDivisor, 1.0f);

		// if current extra fuel portion is not "strong" enough, then we keep up the "pump pressure" with the accumulated portion
		floatms_t maxExtraFuel = std::max(extraFuel, accumulatedValue);
		// use only a fixed fraction of the accumulated portion
		fractionalInjFuel = maxExtraFuel / divisor;

		// update max counters
		maxExtraPerCycle = std::max(extraFuel, maxExtraPerCycle);
		maxInjectedPerPeriod = std::max(fractionalInjFuel, maxInjectedPerPeriod);

		// evenly split it between several engine cycles
		extraFuel = fractionalInjFuel / periodF;
	} else {
		resetFractionValues();
	}

	float mult = interpolate2d(rpm, config->tpsTspCorrValuesBins,
						config->tpsTspCorrValues);
	if (mult != 0 && (mult < 0.01f || mult > 100.f)) {
		mult = 1;
	}

	// Flex fuel transient compensation (CLT x ethanol). Neutral (1.0) without a flex sensor.
	// Predictive MAP and Lua return earlier, so this only applies to legacy adders.
	float flexMult = getFlexTransientMult(config->flexAeMult);
	engine->outputChannels.flexAeMultiplier = flexMult;
	return extraFuel * mult * flexMult;
}

bool TpsAccelEnrichment::setLuaEnrichment(mass_t grams) {
	if (!std::isfinite(grams) || grams < 0) {
		resetLua();
		return false;
	}

	m_luaEnrichment = grams;
	m_luaEnrichmentTimer.reset();
	return true;
}

mass_t TpsAccelEnrichment::getLuaEnrichment() {
	luaAccelFuel = 0;
	if (engineConfiguration->accelEnrichmentMode != AE_MODE_LUA
		|| m_luaEnrichmentTimer.hasElapsedMs(200)
		|| !engine->rpmCalculator.isRunning()
		|| engine->rpmCalculator.getCachedRpm() < engineConfiguration->cranking.rpm
		|| engine->module<DfcoController>()->cutFuel()) {
		return 0;
	}

#if EFI_TUNER_STUDIO
	if (isTuningVeNow()) {
		return 0;
	}
#endif

	luaAccelFuel = m_luaEnrichment;
	if (luaAccelFuel > 0) {
		// Keep closed-loop fuel from undoing the script's transient correction.
		m_timeSinceAccel.reset();
	}
	return luaAccelFuel;
}

void TpsAccelEnrichment::resetLua() {
	m_luaEnrichment = 0;
	luaAccelFuel = 0;
}

void TpsAccelEnrichment::onEngineStop() {
	resetAE();
}

void TpsAccelEnrichment::onEngineCycleTps() {
	onUpdateInvocationCounter++;

	// we used some extra fuel during the current cycle, so we "charge" our "acceleration pump" with it
	accumulatedValue -= maxExtraPerPeriod;
	maxExtraPerPeriod = std::max(maxExtraPerCycle, maxExtraPerPeriod);
	maxExtraPerCycle = 0;
	accumulatedValue += maxExtraPerPeriod;

	// update the accumulated value every 'Period' engine cycles
	isTimeToResetAccumulator = --cycleCnt <= 0;
	if (isTimeToResetAccumulator) {
		maxExtraPerPeriod = 0;

		// we've injected this portion during the cycle, so we set what's left for the next cycle
		accumulatedValue -= maxInjectedPerPeriod;
		maxInjectedPerPeriod = 0;

		// it's an infinitely convergent series, so we set a limit at some point
		// (also make sure that accumulatedValue is positive, for safety)
		static const floatms_t smallEpsilon = 0.001f;
		belowEpsilon = accumulatedValue < smallEpsilon;
		if (belowEpsilon) {
			accumulatedValue = 0;
		}

		// reset the counter
		cycleCnt = engineConfiguration->tpsAccelFractionPeriod;
	}
}

void TpsAccelEnrichment::clearDetector() {
	m_tpsHistoryCount = 0;
	m_rawTpsCount = 0;
	tpsFrom = 0;
	tpsTo = 0;
	deltaTps = 0;
	tpsRate = 0;
	peakTpsRate = 0;
	isAboveAccelThreshold = false;
	isBelowDecelThreshold = false;
	m_accelEventJustOccurred = false;
}

void TpsAccelEnrichment::resetAE() {
	clearDetector();
	extraFuel = 0;
	resetFractionValues();
	resetLua();
}

void TpsAccelEnrichment::resetFractionValues() {
	accumulatedValue = 0;
	maxExtraPerCycle = 0;
	maxExtraPerPeriod = 0;
	maxInjectedPerPeriod = 0;
	cycleCnt = 0;
}

// [tag:tps_rate_detector] TPS opening/closing detector shared by the additive AE
// modes, MAP Prediction (event latch, re-arm and closing exit), burst knock and the
// engine state machine. It runs from Engine::periodicFastCallback() at 200 Hz, before
// the fuel math. With tpsEventConfirmation N, each sample is the median of the last
// 2N-1 raw TPS samples: a spike shorter than N samples disappears in both directions,
// while a held step (for example a TPS received over CAN at a low rate) passes N-1
// samples later. The TPS rate in %/s is measured on these samples over about 10 ms
// (two sample intervals). Thresholds are rates, so they do not depend on the sampling
// period. An opening event starts when the rate exceeds tpsAccelRateThreshold; tpsFrom
// is the TPS at the start of the rate window and tpsTo follows the highest TPS reached.
// The event lasts while the rate stays above the threshold, plus tpsAccelHoldTime.
// Closing events mirror this with tpsDecelRateThreshold. Opening and closing events
// are exclusive: a new one ends the other.
static constexpr float TPS_RATE_MIN_SAMPLE_US = 2000;
// A longer gap (engine stop, sensor dropout) restarts the rate window.
static constexpr float TPS_RATE_MAX_GAP_US = 100000;

static int getEventConfirmationSamples() {
	int samples = config->tpsEventConfirmation;
	// Zero is an unset tune; applyDefaultsOrFixAfterBurn() repairs it to the same default.
	return samples >= 1 && samples <= 3 ? samples : 2;
}

void TpsAccelEnrichment::onInvalidTps() {
	clearDetector();
}

void TpsAccelEnrichment::onNewValue(float currentValue, efitick_t nowNt) {
	if (m_rawTpsCount > 0) {
		float sinceLastUs = NT2USF(nowNt - m_lastRawNt);
		if (sinceLastUs < TPS_RATE_MIN_SAMPLE_US) {
			// Re-entrant fast callback: keep the regular sampling interval.
			return;
		}
		if (sinceLastUs > TPS_RATE_MAX_GAP_US) {
			m_rawTpsCount = 0;
			m_tpsHistoryCount = 0;
		}
	}
	m_lastRawNt = nowNt;

	// Median of the last 2N-1 raw samples. Until that many are collected there is no
	// filtered sample, hence no rate.
	int window = 2 * getEventConfirmationSamples() - 1;
	if (m_rawTpsCount == TPS_MEDIAN_MAX) {
		for (int i = 1; i < TPS_MEDIAN_MAX; i++) {
			m_rawTps[i - 1] = m_rawTps[i];
		}
		m_rawTpsCount--;
	}
	m_rawTps[m_rawTpsCount++] = currentValue;

	bool hasSample = m_rawTpsCount >= window;
	if (hasSample && window > 1) {
		float sorted[TPS_MEDIAN_MAX];
		for (int i = 0; i < window; i++) {
			float value = m_rawTps[m_rawTpsCount - window + i];
			int j = i;
			for (; j > 0 && sorted[j - 1] > value; j--) {
				sorted[j] = sorted[j - 1];
			}
			sorted[j] = value;
		}
		currentValue = sorted[window / 2];
	}

	bool hasRate = hasSample && m_tpsHistoryCount > 0;
	float windowStartTps = currentValue;
	if (hasRate) {
		const TpsSample& oldest = m_tpsHistory[0];
		windowStartTps = oldest.tps;
		float dtSec = NT2USF(nowNt - oldest.nt) * 1e-6f;
		tpsRate = (currentValue - oldest.tps) / dtSec;
	} else {
		tpsRate = 0;
	}

	if (hasSample) {
		// Keep the two most recent filtered samples.
		if (m_tpsHistoryCount == 2) {
			m_tpsHistory[0] = m_tpsHistory[1];
			m_tpsHistoryCount = 1;
		}
		m_tpsHistory[m_tpsHistoryCount++] = { currentValue, nowNt };
	}

	float holdSec = engineConfiguration->tpsAccelHoldTime;
	float accelThreshold = engineConfiguration->tpsAccelRateThreshold;
	float decelThreshold = engineConfiguration->tpsDecelRateThreshold;

	bool opening = hasRate && accelThreshold > 0 && tpsRate > accelThreshold;
	bool closing = hasRate && decelThreshold > 0 && tpsRate < -decelThreshold;

	if (opening) {
		if (!isAboveAccelThreshold) {
			isAboveAccelThreshold = true;
			isBelowDecelThreshold = false;
			tpsFrom = windowStartTps;
			tpsTo = currentValue;
			peakTpsRate = tpsRate;
			// Latched until read once by isAccelEventTriggered().
			m_accelEventJustOccurred = true;
		}
		peakTpsRate = maxF(peakTpsRate, tpsRate);
		m_accelHoldTimer.reset(nowNt);
	} else if (closing) {
		if (!isBelowDecelThreshold) {
			isBelowDecelThreshold = true;
			isAboveAccelThreshold = false;
			tpsFrom = windowStartTps;
			tpsTo = currentValue;
			peakTpsRate = tpsRate;
		}
		peakTpsRate = minF(peakTpsRate, tpsRate);
		m_decelHoldTimer.reset(nowNt);
	}

	if (isAboveAccelThreshold) {
		tpsTo = maxF(tpsTo, currentValue);
		if (!opening && m_accelHoldTimer.getElapsedSeconds(nowNt) >= holdSec) {
			isAboveAccelThreshold = false;
		}
	} else if (isBelowDecelThreshold) {
		tpsTo = minF(tpsTo, currentValue);
		if (!closing && m_decelHoldTimer.getElapsedSeconds(nowNt) >= holdSec) {
			isBelowDecelThreshold = false;
		}
	}

	if (!isAboveAccelThreshold && !isBelowDecelThreshold) {
		tpsFrom = currentValue;
		tpsTo = currentValue;
		peakTpsRate = 0;
	}
	deltaTps = tpsTo - tpsFrom;
}

bool TpsAccelEnrichment::isAccelEventTriggered() {
	// Read the flag
	bool result = m_accelEventJustOccurred;
	// Reset it so we only fire once per event
	m_accelEventJustOccurred = false;
	return result;
}

TpsAccelEnrichment::TpsAccelEnrichment() {
	resetAE();
}

void TpsAccelEnrichment::onConfigurationChange(engine_configuration_s const* /*previousConfig*/) {
	// A transient request belongs to the tune under which it was computed.
	resetLua();
}

float TpsAccelEnrichment::getTimeSinceAcell() const {
	return m_timeSinceAccel.getElapsedSeconds();
}

void initAccelEnrichment() {
	engine->module<TpsAccelEnrichment>()->onConfigurationChange(nullptr);
}
