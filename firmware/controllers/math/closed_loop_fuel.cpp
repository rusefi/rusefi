#include "pch.h"

#include "closed_loop_fuel.h"
#include "closed_loop_fuel_cell.h"
#include "deadband.h"

struct FuelingBank {
	ClosedLoopFuelCellImpl cells[STFT_CELL_COUNT];
};

static FuelingBank banks[STFT_BANK_COUNT];

static Deadband<25> idleDeadband;
static Deadband<2> overrunDeadband;
static Deadband<2> loadDeadband;

static SensorType getSensorForBankIndex(size_t index) {
	switch (index) {
		case 0: return SensorType::Lambda1;
		case 1: return SensorType::Lambda2;
		default: return SensorType::Invalid;
	}
}

size_t computeStftBin(int rpm, float load, stft_s& cfg) {
	// Low RPM -> idle
	if (idleDeadband.lt(rpm, cfg.maxIdleRegionRpm * RPM_1_BYTE_PACKING_MULT))
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

static bool shouldCorrect() {
	const auto& cfg = CONFIG(stft);

	// User disable bit
	if (!CONFIG(fuelClosedLoopCorrectionEnabled)) {
		return false;
	}

	// Don't correct if not running
	if (!ENGINE(rpmCalculator).isRunning()) {
		return false;
	}

	// Startup delay - allow O2 sensor to warm up, etc
	if (cfg.startupDelay > ENGINE(engineState.running.timeSinceCrankingInSecs)) {
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

bool shouldUpdateCorrection(SensorType sensor) {
	const auto& cfg = CONFIG(stft);

	// Pause (but don't reset) correction if the AFR is off scale.
	// It's probably a transient and poorly tuned transient correction
	auto afr = Sensor::get(sensor).value_or(0) * STOICH_RATIO;
	if (!afr || afr < (cfg.minAfr * 0.1f) || afr > (cfg.maxAfr * 0.1f)) {
		return false;
	}

	return true;
}

ClosedLoopFuelResult fuelClosedLoopCorrection() {
	if (!shouldCorrect()) {
		return {};
	}

	size_t binIdx = computeStftBin(GET_RPM(), getFuelingLoad(), CONFIG(stft));

#if EFI_TUNER_STUDIO
	if (engineConfiguration->debugMode == DBG_FUEL_PID_CORRECTION) {
		tsOutputChannels.debugIntField1 = binIdx;
	}
#endif // EFI_TUNER_STUDIO

	ClosedLoopFuelResult result;

	for (int i = 0; i < STFT_BANK_COUNT; i++) {
		auto& cell = banks[i].cells[binIdx];

		SensorType sensor = getSensorForBankIndex(i);

		// todo: push configuration at startup
		cell.configure(&CONFIG(stft.cellCfgs[binIdx]), sensor);

		if (shouldUpdateCorrection(sensor)) {
			cell.update(CONFIG(stft.deadband) * 0.001f, CONFIG(stftIgnoreErrorMagnitude));
		}

		result.banks[i] = cell.getAdjustment();
	}

	return result;
}
