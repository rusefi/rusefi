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

ft_region_e ShortTermFuelTrim::computeStftBin(float rpm, float load, stft_s& cfg) {
	// Low RPM -> idle
	if (idleDeadband.lt(rpm, cfg.maxIdleRegionRpm))
	{
		return ftRegionIdle;
	}

	// Low load -> overrun
	if (overrunDeadband.lt(load, cfg.maxOverrunLoad))
	{
		return ftRegionOverrun;
	}

	// High load -> power
	if (loadDeadband.gt(load, cfg.minPowerLoad))
	{
		return ftRegionPower;
	}

	// Default -> normal "in the middle" cell
	return ftRegionCruise;
}

stft_state_e ShortTermFuelTrim::getCorrectionState() {
	const auto& cfg = engineConfiguration->stft;

	// User disable bit
	if (!engineConfiguration->fuelClosedLoopCorrectionEnabled) {
		return stftDisabledSettings;
	}

	// Don't correct if tuning seems to be happening
	if (checkIfTuningVeNow()) {
		return stftDisabledTuning;
	}

	// Don't correct if not running
	if (!engine->rpmCalculator.isRunning()) {
		return stftDisabledRPM;
	}

	// Startup delay - allow O2 sensor to warm up, etc
	if (cfg.startupDelay > engine->fuelComputer.running.timeSinceCrankingInSecs) {
		return stftDisabledCrankingDelay;
	}

	// Check that the engine is hot enough (and clt not failed)
	auto clt = Sensor::get(SensorType::Clt);
	if (!clt.Valid || clt.Value < cfg.minClt) {
		return stftDisabledClt;
	}

	// If all was well, then we're enabled!
	return stftEnabled;
}

stft_state_e ShortTermFuelTrim::getLearningState(SensorType sensor) {
	const auto& cfg = engineConfiguration->stft;

	// TODO: add check for stftLearningDisabledSettings

	// Pause (but don't reset) correction if the AFR is off scale.
	// It's probably a transient and poorly tuned transient correction
	// TODO: use getStoichiometricRatio() instead of STOICH_RATIO
	auto afr = Sensor::getOrZero(sensor) * STOICH_RATIO;
	if (!afr || afr < cfg.minAfr || afr > cfg.maxAfr) {
		return stftDisabledAfrOurOfRange;
	}

	// Pause correction if DFCO was active recently
	auto timeSinceDfco = engine->module<DfcoController>()->getTimeSinceCut();
	if (timeSinceDfco < engineConfiguration->noFuelTrimAfterDfcoTime) {
		return stftDisabledDFCO;
	}

	// Pause correction if Accel enrichment was active recently
	auto timeSinceAccel = engine->module<TpsAccelEnrichment>()->getTimeSinceAcell();
	if (timeSinceAccel < engineConfiguration->noFuelTrimAfterAccelTime) {
		return stftDisabledTpsAccel;
	}

	// Pause if some other cut was active recently
	auto timeSinceFuelCut = engine->module<LimpManager>()->getTimeSinceAnyCut();
	// TODO: should duration this be configurable?
	if (timeSinceFuelCut < 2) {
		return stftDisabledFuelCut;
	}

	return stftEnabled;
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
	stftCorrectionState = getCorrectionState();
	if (stftCorrectionState != stftEnabled) {
		// Learning is also prohibited
		for (size_t bank = 0; bank < FT_BANK_COUNT; bank++) {
			stftLearningState[bank] = stftCorrectionState;
		}
		return {};
	}

	ClosedLoopFuelResult result;

	result.region = stftCorrectionBinIdx = computeStftBin(rpm, fuelLoad, engineConfiguration->stft);

	for (size_t bank = 0; bank < FT_BANK_COUNT; bank++) {
		auto& cell = banks[bank].cells[stftCorrectionBinIdx];

		SensorType sensor = getSensorForBankIndex(bank);

		stftLearningState[bank] = getLearningState(sensor);
		if (stftLearningState[bank] == stftEnabled) {
			stftInputError[bank] = cell.getLambdaError();
			cell.update(PERCENT_DIV * engineConfiguration->stft.deadband, engineConfiguration->stftIgnoreErrorMagnitude);
			stftLearningBinIdx = stftCorrectionBinIdx;
		}

		result.banks[bank] = cell.getAdjustment();
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
