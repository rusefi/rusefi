#include "pch.h"
#include "accel_enrichment_autotune.h"
#include "flash_main.h"

TpsAccelEnrichmentAutoTune::TpsAccelEnrichmentAutoTune() {
	m_lastAccel.reset();
	cb_lambda.setSize((1000.0f * engineConfiguration->tpsAccelLookback) / FAST_CALLBACK_PERIOD_MS);
}

void TpsAccelEnrichmentAutoTune::onNewValue() {
	m_rpm = Sensor::getOrZero(SensorType::Rpm);
	m_tpsFrom = engine->module<TpsAccelEnrichment>()->getTpsFrom();
	m_tpsTo = engine->module<TpsAccelEnrichment>()->getTpsTo();

	m_lastAccel.reset();
	pendingUpdate = true;
}

void TpsAccelEnrichmentAutoTune::onFastCallback() {
	if(m_lastAccel.getElapsedSeconds() <= engineConfiguration->tpsAccelLookback) {
		if(cb_lambda.getCount() <= cb_lambda.getSize()){
			cb_lambda.add(engine->fuelComputer.targetLambda - Sensor::getOrZero(SensorType::Lambda1));
		}
	}
}

void TpsAccelEnrichmentAutoTune::onSlowCallback() {
	if(pendingUpdate == true && m_lastAccel.getElapsedSeconds() > engineConfiguration->tpsAccelLookback) {
		applyUpdate();
	}
}

void TpsAccelEnrichmentAutoTune::applyUpdate() {
	pendingUpdate = false;

	// Check if the conditions are valid
	float ect = Sensor::getOrZero(SensorType::Clt);

	if(ect >= 82 && m_rpm > 500 && m_rpm < 1400) {
		auto binTpsTo = priv::getBin(m_tpsTo, config->tpsTpsAccelToRpmBins);
		auto binTpsFrom = priv::getBin(m_tpsFrom, config->tpsTpsAccelFromRpmBins);

		auto lowTo = binTpsTo.Idx;
		float fracTo = binTpsTo.Frac;

		auto lowFrom = binTpsFrom.Idx;
		float fracFrom = binTpsFrom.Frac;

		float correction = 0;

		int lambda_count = cb_lambda.getCount();
		float max_lambdadev = cb_lambda.maxValue(lambda_count);
		float min_lambdadev = cb_lambda.minValue(lambda_count);
		float avg_lambdadev = cb_lambda.sum(lambda_count) / float(lambda_count);

		if(abs(avg_lambdadev) > 0.05) {
			if(max_lambdadev > 0.1) {
				correction = 0.05;
			} else if (min_lambdadev < -0.1) {
				correction = -0.05;
			} else {
				correction = avg_lambdadev;
			}
		} else {
			return;
		}

		config->tpsTpsAccelTable[lowTo][lowFrom]     = float(config->tpsTpsAccelTable[lowTo][lowFrom]) *     (1 + correction * (1-fracTo) * (1-fracFrom)); 
		config->tpsTpsAccelTable[lowTo+1][lowFrom]   = float(config->tpsTpsAccelTable[lowTo+1][lowFrom]) *   (1 + correction * (fracTo) * (1-fracFrom)); 
		config->tpsTpsAccelTable[lowTo][lowFrom+1]   = float(config->tpsTpsAccelTable[lowTo][lowFrom+1]) *   (1 + correction * (1-fracTo) * (fracFrom)); 
		config->tpsTpsAccelTable[lowTo+1][lowFrom+1] = float(config->tpsTpsAccelTable[lowTo+1][lowFrom+1]) * (1 + correction * (fracTo) * (fracFrom)); 

		setNeedToWriteConfiguration();
	}

}