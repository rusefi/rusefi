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

float LambdaMonitor::getTimeout() const {
	return engineConfiguration->lambdaProtectionTimeout;
}

bool LambdaMonitorBase::isCut() const {
	return lambdaMonitorCut;
}

void LambdaMonitorBase::update(float rpm, float load) {
	bool isGood = isCurrentlyGood(rpm, load);
	lambdaCurrentlyGood = isGood;
	if (isGood) {
		m_timeSinceGoodLambda.reset();
	}

	lambdaTimeSinceGood = m_timeSinceGoodLambda.getElapsedSeconds();

	if (m_timeSinceGoodLambda.hasElapsedSec(getTimeout())) {
		// Things have been bad long enough, cut!
		lambdaMonitorCut = true;
	}

	if (lambdaMonitorCut) {
		// If things are back to normal, cancel the cut and force a reset
		if (restoreConditionsMet(rpm, load)) {
			lambdaMonitorCut = false;
			m_timeSinceGoodLambda.reset();
		}
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

bool LambdaMonitorBase::restoreConditionsMet(float rpm, float load) const {
	if (rpm > engineConfiguration->lambdaProtectionRestoreRpm) {
		return false;
	}

	if (load > engineConfiguration->lambdaProtectionRestoreLoad) {
		return false;
	}

	if (Sensor::getOrZero(SensorType::Tps1) > engineConfiguration->lambdaProtectionRestoreTps) {
		return false;
	}

	return true;
}
