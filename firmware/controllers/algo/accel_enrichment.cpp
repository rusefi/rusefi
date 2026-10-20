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

	if (engineConfiguration->tpsAccelLookback == 0) {
		// If disabled, return 0.
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
	if (mult != 0 && (mult < 0.01 || mult > 100)) {
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
	// we update values in handleFuel() directly by calling onNewValue()

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

int TpsAccelEnrichment::getMaxDeltaIndex() {
	int len = minI(cb.getSize(), cb.getCount());
	tooShort = len < 2;
	if (tooShort) {
		return 0;
	}
	int ci = cb.currentIndex - 1;
	float maxValue = cb.get(ci) - cb.get(ci - 1);
	int resultIndex = ci;

	// todo: 'get' method is maybe a bit heavy because of the branching
	// todo: this could be optimized with some careful magic

	for (int i = 1; i<len - 1;i++) {
		float v = cb.get(ci - i) - cb.get(ci - i - 1);
		if (v > maxValue) {
			maxValue = v;
			resultIndex = ci - i;
		}
	}

	return resultIndex;
}

float TpsAccelEnrichment::getMaxDelta() {
	int index = getMaxDeltaIndex();

	return (cb.get(index) - (cb.get(index - 1)));
}

void TpsAccelEnrichment::resetAE() {
	cb.clear();
	tpsFrom = 0;
	tpsTo = 0;
	deltaTps = 0;
	isAboveAccelThreshold = false;
	isBelowDecelThreshold = false;
	m_accelEventJustOccurred = false;
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

void TpsAccelEnrichment::setLength(int length) {
	cb.setSize(length);
}

void TpsAccelEnrichment::onNewValue(float currentValue) {
	// Push new value in to the history buffer
	cb.add(currentValue);
	if (cb.getCount() < 2) {
		tpsFrom = currentValue;
		tpsTo = currentValue;
		deltaTps = 0;
		isAboveAccelThreshold = false;
		isBelowDecelThreshold = false;
		return;
	}

	// Update deltas
	int maxDeltaIndex = getMaxDeltaIndex();
	tpsFrom = cb.get(maxDeltaIndex - 1);
	tpsTo = cb.get(maxDeltaIndex);
	deltaTps = tpsTo - tpsFrom;

	// Update threshold detection
	isAboveAccelThreshold = deltaTps > engineConfiguration->tpsAccelEnrichmentThreshold;

	// If an acceleration event just happened, latch the flag so it can be read once.
	if (isAboveAccelThreshold) {
		m_accelEventJustOccurred = true;
	}

	// TODO: can deltaTps actually be negative? Will this ever trigger?
	isBelowDecelThreshold = deltaTps < -engineConfiguration->tpsDecelEnleanmentThreshold;
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
	cb.setSize(4);
}

void TpsAccelEnrichment::onConfigurationChange(engine_configuration_s const* /*previousConfig*/) {
	// A transient request belongs to the tune under which it was computed.
	resetLua();

	if (engineConfiguration->accelEnrichmentMode == AE_MODE_LUA) {
		// The script owns event detection; don't depend on legacy lookback settings.
		return;
	}

	constexpr float slowCallbackPeriodSecond = SLOW_CALLBACK_PERIOD_MS / 1000.0f;
	int length;

	if (engineConfiguration->accelEnrichmentMode == AE_MODE_PREDICTIVE_MAP) {
		// In predictive MAP mode, size the lookback buffer to cover the longest possible
		// blend duration so the TPS history window is consistent with the prediction window.
		float maxBlendDuration = 0;
		for (int i = 0; i < TPS_TPS_ACCEL_CLT_CORR_TABLE; i++) {
			maxBlendDuration = maxF(maxBlendDuration, config->predictiveMapBlendDurationValues[i]);
		}
		length = maxF(2, ceilf(maxBlendDuration / slowCallbackPeriodSecond));
	} else {
		length = engineConfiguration->tpsAccelLookback / slowCallbackPeriodSecond;

		if (length < 1) {
			efiPrintf("setTpsAccelLen: Length should be positive [%d]", length);
			return;
		}
	}

	setLength(length);
}

float TpsAccelEnrichment::getTimeSinceAcell() const {
	return m_timeSinceAccel.getElapsedSeconds();
}

void initAccelEnrichment() {
	engine->module<TpsAccelEnrichment>()->onConfigurationChange(nullptr);
}
