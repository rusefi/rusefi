#include "pch.h"

#include "closed_loop_fuel.h"
#include "tunerstudio.h"

#if EFI_ENGINE_CONTROL

SensorType ShortTermFuelTrim::getSensorForBankIndex(size_t index) {
	switch (index) {
		case 0: return SensorType::Lambda1;
		case 1: return SensorType::Lambda2;
		default: return SensorType::Invalid;
	}
}

size_t ShortTermFuelTrim::computeStftBin(float rpm, float load, stft_s& cfg) {
	// Low RPM -> idle
	if (idleDeadband.lt(rpm, cfg.maxIdleRegionRpm))
	{
		return 0;
	}

	// Low load -> overrun
	if (overrunDeadband.lt(load, cfg.maxOverrunLoad))
	{
		return 1;
	}

	// High load -> power
	if (loadDeadband.gt(load, cfg.minPowerLoad))
	{
		return 2;
	}

	// Default -> normal "in the middle" cell
	return 3;
}

bool ShortTermFuelTrim::shouldCorrect() {
	const auto& cfg = engineConfiguration->stft;

	// User disable bit
	if (!engineConfiguration->fuelClosedLoopCorrectionEnabled) {
		return false;
	}

	// Don't correct if tuning seems to be happening
	if (checkIfTuningVeNow()) {
		return false;
	}

	// Don't correct if not running
	if (!engine->rpmCalculator.isRunning()) {
		return false;
	}

	// Startup delay - allow O2 sensor to warm up, etc
	if (cfg.startupDelay > engine->fuelComputer.running.timeSinceCrankingInSecs) {
		return false;
	}

	// Check that the engine is hot enough (and clt not failed)
	auto clt = Sensor::get(SensorType::Clt);
	if (!clt.Valid || clt.Value < cfg.minClt) {
		return false;
	}

	// If all was well, then we're enabled!
	return true;
}

bool ShortTermFuelTrim::shouldUpdateCorrection(SensorType sensor) {
	const auto& cfg = engineConfiguration->stft;

	// Pause (but don't reset) correction if the AFR is off scale.
	// It's probably a transient and poorly tuned transient correction
	auto afr = Sensor::getOrZero(sensor) * STOICH_RATIO;
	if (!afr || afr < cfg.minAfr || afr > cfg.maxAfr) {
		return false;
	}

	// Pause correction if DFCO was active recently
	auto timeSinceDfco = engine->module<DfcoController>()->getTimeSinceCut();
	if (timeSinceDfco < engineConfiguration->noFuelTrimAfterDfcoTime) {
		return false;
	}

	// Pause correction if Accel enrichment was active recently
	auto timeSinceAccel = engine->module<TpsAccelEnrichment>()->getTimeSinceAcell();
	if (timeSinceAccel < engineConfiguration->noFuelTrimAfterAccelTime) {
		return false;
	}

	// Pause if some other cut was active recently
	auto timeSinceFuelCut = engine->module<LimpManager>()->getTimeSinceAnyCut();
	// TODO: should duration this be configurable?
	if (timeSinceFuelCut < 2) {
		return false;
	}

	return true;
}

void ShortTermFuelTrim::init(stft_s *stftCfg) {
	for (size_t bank = 0; bank < FT_BANK_COUNT; bank++) {
		for (size_t bin = 0; bin < STFT_CELL_COUNT; bin++) {
			auto& cell = banks[bank].cells[bin];
			SensorType sensor = getSensorForBankIndex(bank);

			cell.configure(&stftCfg->cellCfgs[bin], sensor);
		}
	}
}

ClosedLoopFuelResult ShortTermFuelTrim::getCorrection(float rpm, float fuelLoad) {
	if (!shouldCorrect()) {
		stftBinIdx = 5;
		stftLearningBinIdx = 5;
		return {};
	}

	stftBinIdx = computeStftBin(rpm, fuelLoad, engineConfiguration->stft);

	ClosedLoopFuelResult result;

	for (int i = 0; i < FT_BANK_COUNT; i++) {
		auto& cell = banks[i].cells[stftBinIdx];

		SensorType sensor = getSensorForBankIndex(i);

		if (shouldUpdateCorrection(sensor)) {
			cell.update(engineConfiguration->stft.deadband * 0.01f, engineConfiguration->stftIgnoreErrorMagnitude);
			stftLearningBinIdx = stftBinIdx;
		} else {
			stftLearningBinIdx = 5;
		}

		result.banks[i] = cell.getAdjustment();
	}

	return result;
}

void ShortTermFuelTrim::onSlowCallback() {
	// Do some magic math here?
}

bool ShortTermFuelTrim::needsDelayedShutoff() {
	return false;
}

void initStft(void)
{
	engine->module<ShortTermFuelTrim>()->init(&engineConfiguration->stft);
}

/* TODO: move out of here */
bool checkIfTuningVeNow() {
#if EFI_TUNER_STUDIO
	const bool result = isTuningVeNow();
#else
	const bool result = false;
#endif /* EFI_TUNER_STUDIO */
	engine->outputChannels.isTuningNow = result;
	return result;
}

#endif // EFI_ENGINE_CONTROL
