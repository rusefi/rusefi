#include "pch.h"
#include "closed_loop_fuel.h"
#include "closed_loop_fuel_cell.h"
#include "deadband.h"
#include "tunerstudio.h"
#include "engine_math.h"
#include "efi_gpio.h"
#include "event_queue.h"
#include "efitime.h"
#include "ignition_controller.h"

#if EFI_ENGINE_CONTROL

struct FuelingBank {
	ClosedLoopFuelCellImpl cells[STFT_CELL_COUNT];
};

static FuelingBank banks[STFT_BANK_COUNT];

static Deadband<25> idleDeadband;
static Deadband<2> overrunDeadband;
static Deadband<2> loadDeadband;

void LongTermFuelTrim::resetLtftTimer() {
	lastLtftUpdateTime = uint32_t(getTimeNowS);
	// updatedLtft = 0;
}

float LongTermFuelTrim::filterStft(float stftRaw) {
	// Filtro EMA: stftEma = alpha * stftRaw + (1 - alpha) * stftEma
	float alpha = (float)config->ltftEmaAlpha / 255.0f;
	stftEma = alpha * stftRaw + (1.0f - alpha) * stftEma;
	return stftEma;
}

void LongTermFuelTrim::updateTimers(bool ignitionState) {
	uint32_t now = getTimeNowS();
	if (ignitionState) {
		if (!ignitionOnDelayActive) {
			ignitionOnTimestamp = now;
			ignitionOnDelayActive = true;
		}
		// Se já passou o delay, desativa o bloqueio
		if (ignitionOnDelayActive && (now - ignitionOnTimestamp) >= config->ltftIgnitionOnDelay) {
			ignitionOnDelayActive = false;
		}
		// Cancelar delay de salvamento se ligou de novo
		ignitionOffSaveDelayActive = false;
		pendingSave = false;
	} else {
		if (!ignitionOffSaveDelayActive) {
			ignitionOffTimestamp = now;
			ignitionOffSaveDelayActive = true;
			pendingSave = updatedLtft;
		}
		// Se já passou o delay, salva se necessário
		if (ignitionOffSaveDelayActive && (now - ignitionOffTimestamp) >= config->ltftIgnitionOffSaveDelay) {
			if (pendingSave) {
				copyTable(config->ltftTable, ltftTableHelper);
				setNeedToWriteConfiguration();
				updatedLtft = 0;
				pendingSave = false;
			}
			ignitionOffSaveDelayActive = false;
		}
	}
}

bool LongTermFuelTrim::canLearn() {
	// Só aprende se não estiver no delay pós-ignição ON
	if (ignitionOnDelayActive) return false;
	// Outras condições já existentes (temperatura, etc)
	if (!config->ltftEnabled) return false;
	if ((Sensor::get(SensorType::Clt)).value_or(0) < float(config->ltftMinModTemp)) return false;
	return true;
}

void LongTermFuelTrim::applyRegionalCorrection(float load, float rpm, float correction) {
	// Exemplo: aplicar correção em toda a faixa de RPM se padrão consistente detectado
	if(load < 10.0f) return;
	int binRpm = priv::getBin(rpm, config->veRpmBins).Idx;
	int window = config->ltftRegionalWindow;
	float intensity = (float)config->ltftRegionalIntensity / 100.0f;
	for (int i = 0; i < 16; i++) {
		if (abs(i - binRpm) <= window/2) {
			for (int j = 0; j < 16; j++) {
				float weight = 1.0f - (float)abs(i - binRpm) / (window/2 + 1);
				ltftTableHelper[j][i] *= (1.0f + correction * weight * intensity);
			}
		}
	}
}

void LongTermFuelTrim::smoothHoles() {
	// Detectar buracos e suavizar
	float threshold = (float)config->ltftHoleThreshold;
	float intensity = (float)config->ltftSmoothingIntensity / 100.0f;
	for (int i = 1; i < 15; i++) {
		for (int j = 1; j < 15; j++) {
			float v = ltftTableHelper[i][j];
			float avg = (ltftTableHelper[i-1][j] + ltftTableHelper[i+1][j] + ltftTableHelper[i][j-1] + ltftTableHelper[i][j+1]) / 4.0f;
			if (fabsf(v - avg) > threshold) {
				ltftTableHelper[i][j] = v * (1.0f - intensity) + avg * intensity;
			}
		}
	}
}

// Função utilitária para checar se a ignição está ligada
static bool isIgnitionOn() {
	auto ign = engine->module<IgnitionController>()->secondsSinceIgnVoltage() > 1.0f;
	return ign;
}

void LongTermFuelTrim::updateLtft(float load, float rpm) {
	// Permitir aprendizado imediato
	if (!config->ltftEnabled) return;
	if ((Sensor::get(SensorType::Clt)).value_or(0) < float(config->ltftMinModTemp)) return;
	auto binLoad = priv::getBin(load, config->veLoadBins);
	auto binRpm = priv::getBin(rpm, config->veRpmBins);
	int lowLoad = binLoad.Idx;
	float fracLoad = binLoad.Frac;
	int lowRpm = binRpm.Idx;
	float fracRpm = binRpm.Frac;
	if (lowLoad > 14 || lowRpm > 14 || fracLoad <= 0.0f || fracLoad >= 1.0f || fracRpm <= 0.0f || fracRpm >= 1.0f) return;
	float stftRaw = engine->engineState.stftCorrection[0] - 1.0f;
	float stftFiltered = filterStft(stftRaw);
	if (fabsf(stftFiltered) > (float)config->ltftStftRejectThreshold / 100.0f) return;
	auto lambda = Sensor::get(SensorType::Lambda1);
	float lambdaError = lambda.Value - engine->fuelComputer.targetLambda;
	if ((stftFiltered > 0.0f && lambdaError < 0.0f) || (stftFiltered < 0.0f && lambdaError > 0.0f)) return;
	float correctionRate = interpolate3d(
		config->ltftCorrectionRate,
		config->veLoadBins, load,
		config->veRpmBins, rpm
	) * 0.01f;
	float correction = correctionRate * 0.005f * (stftFiltered / (fabsf(stftFiltered))) * (1 - powf(10, -20 * (100.0f / config->ltftPermissivity) * fabsf(stftFiltered)));
	if (fabsf(correction) > fabsf(stftFiltered)) {
		correction = stftFiltered * stftFiltered / fabsf(stftFiltered);
	}
	if (fabsf(correction) <= 0.2f) {
		// Correção regional se padrão consistente
		if (fabsf(stftFiltered) > 0.05f) {
			applyRegionalCorrection(load, rpm, correction);
		}
		// Correção bilinear padrão
		ltftTableHelper[lowLoad][lowRpm]     *= (1 + correction * (1-fracLoad) * (1-fracRpm));
		ltftTableHelper[lowLoad+1][lowRpm]   *= (1 + correction * (fracLoad) * (1-fracRpm));
		ltftTableHelper[lowLoad][lowRpm+1]   *= (1 + correction * (1-fracLoad) * (fracRpm));
		ltftTableHelper[lowLoad+1][lowRpm+1] *= (1 + correction * (fracLoad) * (fracRpm));
		// Clamping
		for(int i = 0; i < 2; i++){
			for (int j = 0; j < 2; j++) {
				if(ltftTableHelper[lowLoad+i][lowRpm+j] > float(100.0f + float(config->ltftMaxCorrection))) {
					ltftTableHelper[lowLoad+i][lowRpm+j] = float(100.0f + float(config->ltftMaxCorrection));
				} else if (ltftTableHelper[lowLoad+i][lowRpm+j] < float(100.0f - float(config->ltftMinCorrection))) {
					ltftTableHelper[lowLoad+i][lowRpm+j] = float(100.0f - float(config->ltftMinCorrection));
				}
			}
		}
		// Após atualização da tabela, salvar imediatamente
		updatedLtft = 1;
		smoothHoles();
		copyTable(config->ltftTable, ltftTableHelper);
		setNeedToWriteConfiguration();
	}
}

void LongTermFuelTrim::onIgnitionStateChanged(bool ignitionState) {
	updateTimers(ignitionState);
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

LongTermFuelTrim::LongTermFuelTrim() {
	stftEma = 1.0f;
	ignitionOnTimestamp = 0;
	ignitionOffTimestamp = 0;
	ignitionOnDelayActive = false;
	ignitionOffSaveDelayActive = false;
	pendingSave = false;
	for (int i = 0; i < 16; i++) {
		regionalErrorBuffer[i] = 0.0f;
		regionalErrorCount[i] = 0;
		for (int j = 0; j < 16; j++) {
			ltftTableHelper[i][j] = 100.0f;
		}
	}
	ltftTableHelperInit = 1;
}

#endif // EFI_ENGINE_CONTROL
