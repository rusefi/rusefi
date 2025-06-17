#include "pch.h"

#include "closed_loop_fuel.h"
#include "closed_loop_fuel_cell.h"
#include "deadband.h"
#include "tunerstudio.h"

#if EFI_ENGINE_CONTROL

struct FuelingBank {
	ClosedLoopFuelCellImpl cells[STFT_CELL_COUNT];
};

static FuelingBank banks[FT_BANK_COUNT];

static Deadband<25> idleDeadband;
static Deadband<2> overrunDeadband;
static Deadband<2> loadDeadband;

namespace {
	SensorType getSensorForBankIndex(size_t index) {
		switch (index) {
			case 0: return SensorType::Lambda1;
			case 1: return SensorType::Lambda2;
			default: return SensorType::Invalid;
		}
	}
}

size_t computeStftBin(float rpm, float load, stft_s& cfg) {
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

namespace {
	bool checkIfTuningIsNow() {
#if EFI_TUNER_STUDIO
		const bool result = isTuningNow();
#else
		const bool result = false;
#endif /* EFI_TUNER_STUDIO */
		engine->outputChannels.isTuningNow = result;
		return result;
	}

	bool shouldCorrect() {
		const auto& cfg = engineConfiguration->stft;

		// User disable bit
		if (!engineConfiguration->fuelClosedLoopCorrectionEnabled) {
			return false;
		}

		// Don't correct if tuning seens to be happening
		if (checkIfTuningIsNow()) {
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
}

bool shouldUpdateCorrection(SensorType sensor) {
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

ClosedLoopFuelResult fuelClosedLoopCorrection() {
	if (!shouldCorrect()) {
#if EFI_TUNER_STUDIO
		engine->outputChannels.fuelClosedLoopBinIdx = 5;
		engine->outputChannels.fuelClosedLoopLearningBinIdx = 5;
#endif // EFI_TUNER_STUDIO
		return {};
	}

	size_t binIdx = computeStftBin(Sensor::getOrZero(SensorType::Rpm), getFuelingLoad(), engineConfiguration->stft);

#if EFI_TUNER_STUDIO
	engine->outputChannels.fuelClosedLoopBinIdx = binIdx;
#endif // EFI_TUNER_STUDIO

	ClosedLoopFuelResult result;

	for (int i = 0; i < FT_BANK_COUNT; i++) {
		auto& cell = banks[i].cells[binIdx];

		SensorType sensor = getSensorForBankIndex(i);

		// todo: push configuration at startup
		cell.configure(&engineConfiguration->stft.cellCfgs[binIdx], sensor);

		if (shouldUpdateCorrection(sensor)) {
			cell.update(engineConfiguration->stft.deadband * 0.01f, engineConfiguration->stftIgnoreErrorMagnitude);
			engine->outputChannels.fuelClosedLoopLearningBinIdx = binIdx;
		} else {
			engine->outputChannels.fuelClosedLoopLearningBinIdx = 5;
		}

		result.banks[i] = cell.getAdjustment();
	}

	return result;
}

#endif // EFI_ENGINE_CONTROL
