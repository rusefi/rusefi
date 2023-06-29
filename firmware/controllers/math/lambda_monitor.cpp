#include "pch.h"

#include "lambda_monitor.h"

float LambdaMonitor::getMaxAllowedLambda(float rpm, float load) const {
	return
		engine->fuelComputer.targetLambda
		+ interpolate3d(
			config->lambdaMaxDeviationTable,
			config->lambdaMaxDeviationLoadBins, load,
			config->lambdaMaxDeviationRpmBins, rpm
		);
}

void LambdaMonitorBase::update(float rpm, float load) {
	if (isCurrentlyGood(rpm, load)) {
		m_timeSinceGoodLambda.reset();
	}

	float timeout = engineConfiguration->lambdaProtectionTimeout;

	if (m_timeSinceGoodLambda.hasElapsedSec(timeout)) {
		// Things have been bad long enough, respond!

		// TODO
	}
}


bool LambdaMonitorBase::isCurrentlyGood(float rpm, float load) const {
	// Lambda is always good if disabled
	if (!engineConfiguration->lambdaProtectionEnable) {
		return true;
	}

	// Below min RPM, don't check
	if (rpm < engineConfiguration->lambdaProtectionMinRpm) {
		return true;
	}

	// Below min load, don't check
	if (load < engineConfiguration->lambdaProtectionMinLoad) {
		return true;
	}

	// Below min TPS, don't check
	if (Sensor::getOrZero(SensorType::Tps1) <= engineConfiguration->lambdaProtectionMinTps) {
		return true;
	}

	// Pause checking if DFCO was active recently
	auto timeSinceDfco = engine->module<DfcoController>()->getTimeSinceCut();
	if (timeSinceDfco < engineConfiguration->noFuelTrimAfterDfcoTime) {
		return true;
	}

	// Pause checking if some other cut was active recently
	auto timeSinceFuelCut = engine->module<LimpManager>()->getTimeSinceAnyCut();
	// TODO: should this duration be configurable?
	if (timeSinceFuelCut < 2) {
		return true;
	}

	// TODO: multiple banks
	if (auto lambda = Sensor::get(SensorType::Lambda1)) {
		if (lambda.Value < getMaxAllowedLambda(rpm, load)) {
			// Lambda is OK, we're good.
			return true;
		}
	} else {
		// Broken lambda sensor doesn't imply bad lambda

		// TODO: can/should we be smarter here?
		return true;
	}

	// All checks failed, lambda is currently bad.
	return false;
}
