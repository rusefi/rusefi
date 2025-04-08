#include "pch.h"

#include "closed_loop_fuel.h"
#include "closed_loop_fuel_cell.h"
#include "deadband.h"
#include "flash_main.h"
#include "tunerstudio.h"
#include "table_helper.h"

#if EFI_ENGINE_CONTROL

struct FuelingBank {
	ClosedLoopFuelCellImpl cells[STFT_CELL_COUNT];
};

static FuelingBank banks[STFT_BANK_COUNT];

static Deadband<25> idleDeadband;
static Deadband<2> overrunDeadband;
static Deadband<2> loadDeadband;

void LongTermFuelTrim::resetLtftTimer() {
	lastLtftUpdateTime = uint32_t(getTimeNowMs);
	// updatedLtft = 0;
}

void LongTermFuelTrim::updateLtft(float load, float rpm) {
	auto binLoad = priv::getBin(load, config->veLoadBins);
	auto binRpm = priv::getBin(rpm, config->veRpmBins);

	if(config->ltftEnabled) {

		if((Sensor::get(SensorType::Clt)).value_or(0) > float(config->ltftMinModTemp)) {

			auto lowLoad = binLoad.Idx;
			float fracLoad = binLoad.Frac;

			auto lowRpm = binRpm.Idx;
			float fracRpm = binRpm.Frac;

			if(lowLoad <= 14 && lowRpm <= 14 && fracLoad > 0.00f && fracLoad < 1.00f && fracRpm > 0.00f && fracRpm < 1.00f) {

				float stftCorrection = engine->engineState.stftCorrection[0] - 1.00f;

				auto lambda = Sensor::get(SensorType::Lambda1);
				float lambdaError = lambda.Value - engine->fuelComputer.targetLambda;

				// Only correct if stft on correct path
				if((stftCorrection > 0.0f && lambdaError < 0.0f) || (stftCorrection < 0.0f && lambdaError > 0.0f)) {
					return;
				}

				float correctionRate = interpolate3d(
										config->ltftCorrectionRate,
										config->veLoadBins, load,
										config->veRpmBins, rpm
									) * 0.01f;

				float correction = correctionRate * 0.005f * (stftCorrection / (abs(stftCorrection))) * (1 - pow(10, - 20 * (100 / config->ltftPermissivity) * abs(stftCorrection)));	// fast callback occours at 200Hz frequency
				
				if(abs(correction) > abs(stftCorrection)) {
					correction = stftCorrection * stftCorrection / (abs(stftCorrection));
				}

				if(abs(correction) <= 0.2) {
					ltftTableHelper[lowLoad][lowRpm]     = float(ltftTableHelper[lowLoad][lowRpm]) *     (1 + correction * (1-fracLoad) * (1-fracRpm)); 
					ltftTableHelper[lowLoad+1][lowRpm]   = float(ltftTableHelper[lowLoad+1][lowRpm]) *   (1 + correction * (fracLoad) * (1-fracRpm)); 
					ltftTableHelper[lowLoad][lowRpm+1]   = float(ltftTableHelper[lowLoad][lowRpm+1]) *   (1 + correction * (1-fracLoad) * (fracRpm)); 
					ltftTableHelper[lowLoad+1][lowRpm+1] = float(ltftTableHelper[lowLoad+1][lowRpm+1]) * (1 + correction * (fracLoad) * (fracRpm)); 

					for(int i = 0; i < 2; i++){
						for (int j = 0; j < 2; j++) {
							if(ltftTableHelper[lowLoad+i][lowRpm+j] > float(100.0f + float(config->ltftMaxCorrection))) {
								ltftTableHelper[lowLoad+i][lowRpm+j] = float(100.0f + float(config->ltftMaxCorrection));
							} else if (ltftTableHelper[lowLoad+i][lowRpm+j] < float(100.0f - float(config->ltftMinCorrection))) {
								ltftTableHelper[lowLoad+i][lowRpm+j] = float(100.0f - float(config->ltftMinCorrection));
							}
						}
					}

					updatedLtft = 1;
				}
			}

		}

	}

}

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

ClosedLoopFuelResult fuelStftClosedLoopCorrection() {
	if (!shouldCorrect()) {
		return {};
	}

	size_t binIdx = computeStftBin(Sensor::getOrZero(SensorType::Rpm), getFuelingLoad(), engineConfiguration->stft);

#if EFI_TUNER_STUDIO
	engine->outputChannels.fuelClosedLoopBinIdx = binIdx;
#endif // EFI_TUNER_STUDIO

	ClosedLoopFuelResult result;

	for (int i = 0; i < STFT_BANK_COUNT; i++) {
		auto& cell = banks[i].cells[binIdx];

		SensorType sensor = getSensorForBankIndex(i);
		cell.configure(&engineConfiguration->stft.cellCfgs[binIdx], sensor);

		// todo: push configuration at startup
		if (shouldUpdateCorrection(sensor)) {
			cell.update(engineConfiguration->stft.deadband * 0.01f, engineConfiguration->stftIgnoreErrorMagnitude);
		}

		result.banks[i] = cell.getAdjustment();
	}

	return result;
}

float LongTermFuelTrim::getLtft(float load, float rpm) {

	if(config->ltftCRC != 132) {
		setTable(config->ltftTable, 100);
		config->ltftCRC = 132;
		ltftTableHelperInit = 0;
		setNeedToWriteConfiguration();
	}

	if(!ltftTableHelperInit){
		copyTable(ltftTableHelper, config->ltftTable, 1);
		ltftTableHelperInit = 1;
	}

	if(shouldUpdateCorrection(getSensorForBankIndex(0)) && shouldCorrect()) {
		updateLtft(load, rpm);
	} else {
		resetLtftTimer();
	}

	if(rpm == 0 && updatedLtft) {
		copyTable(config->ltftTable, ltftTableHelper);
		setNeedToWriteConfiguration();
		updatedLtft = 0;
	}

	if(config->ltftEnabled && config->ltftCRC == 132 && (Sensor::get(SensorType::Clt)).value_or(0) > float(config->ltftMinTemp)) {
		float ltft = interpolate3d(ltftTableHelper,
			  config->veLoadBins, load,
			  config->veRpmBins, rpm
		) * 0.01f;

    /*
		if(100.0f * ltft > config->ltftMaxCorrection || 100.0f * ltft < config->ltftMinCorrection) {
			config->ltftEnabled = 0;
			return 1.00f;
		}
    */

		return ltft;
	} else {
		return 1.00f;
	}
}

#endif // EFI_ENGINE_CONTROL
