/**
 * @file transient_timing.cpp
 *
 * [tag:transient_timing] Signed ignition timing correction during throttle transients.
 *
 * The AE TPS detector (TpsAccelEnrichment) reports opening and closing events
 * (isAboveAccelThreshold, isBelowDecelThreshold) and their TPS change (deltaTps). While an
 * opening event is active the correction follows the table at deltaTps and RPM; while a
 * closing event is active it follows the closing curve at RPM. After the event it stays
 * at the last value for the hold time, then ramps linearly to zero over the decay time.
 * A new event replaces the old one. Positive values advance, negative values retard.
 *
 * Limits: the result is clamped to the configured maximum advance and retard, and never
 * advances while knock control is retarding. It is zero while cranking or stopped, and
 * Log only computes it without applying it. Engine::periodicFastCallback() calls update()
 * before the timing math, which adds getTimingCorrection() in
 * getAdvanceCorrections() (so fixed timing and the cranking-correction switch apply).
 *
 * Each cylinder's spark angle is prepared when its previous spark fires, so a new
 * correction reaches a cylinder at its next prepared spark: up to one engine cycle later
 * (see TransientTimingIntegration in test_transient_timing.cpp).
 */

#include "pch.h"
#include "transient_timing.h"

void setDefaultTransientTiming() {
	config->transientTimingMode = TRANSIENT_TIMING_OFF;
	const uint8_t tpsChange[] = {5, 20, 50, 80};
	for (size_t i = 0; i < efi::size(config->transientTimingTpsChangeBins); i++) {
		config->transientTimingTpsChangeBins[i] = tpsChange[i];
	}
	setLinearCurve(config->transientTimingRpmBins, 1000, 4000, 50);
	setTable(config->transientTimingOpeningTable, 0);
	setArrayValues(config->transientTimingClosingValues, 0);
	config->transientTimingHoldMs = 100;
	config->transientTimingDecayMs = 300;
	config->transientTimingMaxAdvance = 5;
	config->transientTimingMaxRetard = 15;
}

void TransientTiming::setDefaultConfiguration() {
	setDefaultTransientTiming();
}

void TransientTiming::reset() {
	m_state = State::Idle;
	m_eventValue = 0;
	m_target = 0;
	m_applied = 0;
}

void TransientTiming::onEngineStop() {
	reset();
}

void TransientTiming::update(efitick_t nowNt) {
	auto& ign = engine->ignitionState;
	if (config->transientTimingMode == TRANSIENT_TIMING_OFF
			|| !engine->rpmCalculator.isRunning() || engine->rpmCalculator.isCranking()) {
		reset();
	} else {
		auto& ae = engine->module<TpsAccelEnrichment>().unmock();
		float rpm = Sensor::getOrZero(SensorType::Rpm);
		float value = 0;
		if (ae.isAboveAccelThreshold) {
			m_eventValue = interpolate3d(config->transientTimingOpeningTable,
				config->transientTimingTpsChangeBins, ae.deltaTps,
				config->transientTimingRpmBins, rpm);
			m_state = State::Opening;
			m_holdTimer.reset(nowNt);
			value = m_eventValue;
		} else if (ae.isBelowDecelThreshold) {
			m_eventValue = interpolate2d(rpm, config->transientTimingRpmBins, config->transientTimingClosingValues);
			m_state = State::Closing;
			m_holdTimer.reset(nowNt);
			value = m_eventValue;
		} else if (m_state != State::Idle) {
			float elapsedMs = m_holdTimer.getElapsedSeconds(nowNt) * 1000;
			float holdMs = config->transientTimingHoldMs;
			float decayMs = config->transientTimingDecayMs;
			if (elapsedMs < holdMs) {
				m_state = State::Hold;
				value = m_eventValue;
			} else if (elapsedMs < holdMs + decayMs) {
				m_state = State::Decay;
				value = m_eventValue * (1 - (elapsedMs - holdMs) / decayMs);
			} else {
				m_state = State::Idle;
				m_eventValue = 0;
			}
		}

		if (!std::isfinite(value)) {
			value = 0;
		}
		value = clampF(-config->transientTimingMaxRetard, value, config->transientTimingMaxAdvance);
		if (value > 0 && engine->module<KnockController>()->getKnockRetard() > 0) {
			// Never add advance while knock control is pulling timing.
			value = 0;
		}
		m_target = value;
		m_applied = config->transientTimingMode == TRANSIENT_TIMING_APPLY ? m_target : 0;
	}

	ign.transientTimingTarget = m_target;
	ign.transientTimingCorrection = m_applied;
	ign.transientTimingState = static_cast<uint8_t>(m_state);
}
