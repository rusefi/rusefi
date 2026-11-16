#include "pch.h"
#include "burst_knock.h"
#include "accel_enrichment.h"

void BurstKnock::setDefaultConfiguration() {
	engineConfiguration->burstKnockEnabled = false;
	engineConfiguration->burstKnockDecayTime = 1.0f;
	setLinearCurve(config->burstKnockRpmBins, 800, 6400);
	setLinearCurve(config->burstKnockTpsRateBins, 0, 1050);
	setTable(config->burstKnockRetardTable, 0);
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
	// [tag:tps_rate_detector] This runs at 20 Hz while the detector samples at 200 Hz:
	// use the event's peak opening rate so a short stab is not missed between samples.
	burstKnockTpsRate = ae.isAboveAccelThreshold ? ae.peakTpsRate : ae.tpsRate;
	float rpm = Sensor::getOrZero(SensorType::Rpm);

	if (ae.isAboveAccelThreshold) {
		float retard = interpolate3d(config->burstKnockRetardTable,
			config->burstKnockTpsRateBins, burstKnockTpsRate,
			config->burstKnockRpmBins, rpm);
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
