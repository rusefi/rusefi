#include "pch.h"
#include "burst_knock.h"
#include "accel_enrichment.h"

void BurstKnock::setDefaultConfiguration() {
	engineConfiguration->burstKnockEnabled = false;
	engineConfiguration->burstKnockDecayTime = 1.0f;
	setLinearCurve(engineConfiguration->burstKnockRpmBins, 800, 6400);
	setLinearCurve(engineConfiguration->burstKnockTpsRateBins, 0, 1050);
	setTable(engineConfiguration->burstKnockRetardTable, 0);
}

void BurstKnock::onSlowCallback() {
	auto cfg = engineConfiguration;

	if (!cfg->burstKnockEnabled) {
		isBurstKnockActive = false;
		burstKnockRetard = 0;
		burstKnockTpsRate = 0;
		m_latchedRetard = 0;
		return;
	}

	auto& ae = engine->module<TpsAccelEnrichment>().unmock();
	// AE supplies the largest adjacent TPS jump in its lookback buffer. This is a
	// rate proxy using the configured lookback, not the instantaneous TPS derivative.
	float lookback = maxF(engineConfiguration->tpsAccelLookback, 0.001f);
	burstKnockTpsRate = ae.deltaTps / lookback;
	float rpm = Sensor::getOrZero(SensorType::Rpm);

	if (ae.isAboveAccelThreshold) {
		float retard = interpolate3d(cfg->burstKnockRetardTable,
			cfg->burstKnockTpsRateBins, burstKnockTpsRate,
			cfg->burstKnockRpmBins, rpm);
		// Retard only: negative cells must never advance timing.
		m_latchedRetard = retard < 0 ? 0 : retard;
		burstKnockRetard = m_latchedRetard;
		isBurstKnockActive = m_latchedRetard > 0;
		m_decayTimer.reset();
	} else if (isBurstKnockActive) {
		// Decay from the last table value and the last above-threshold sample.
		float decayTime = cfg->burstKnockDecayTime;
		float elapsed = m_decayTimer.getElapsedSeconds();
		if (decayTime <= 0 || elapsed >= decayTime) {
			isBurstKnockActive = false;
			burstKnockRetard = 0;
			m_latchedRetard = 0;
		} else {
			burstKnockRetard = m_latchedRetard * (1 - elapsed / decayTime);
		}
	} else {
		burstKnockRetard = 0;
	}
}

float BurstKnock::getTimingRetard() const {
	return isBurstKnockActive ? burstKnockRetard : 0;
}
