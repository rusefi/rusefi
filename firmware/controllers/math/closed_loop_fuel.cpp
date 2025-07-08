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
	m_updateTimer.reset();
}

float LongTermFuelTrim::filterStft(float stftRaw) {
	// Filtro EMA: stftEma = alpha * stftRaw + (1 - alpha) * stftEma
	float alpha = (float)config->ltftEmaAlpha / 255.0f;
	stftEma = alpha * stftRaw + (1.0f - alpha) * stftEma;
	return stftEma;
}

bool LongTermFuelTrim::canLearn() {
	// Só aprende se passou tempo suficiente desde ignição ON
	if (!m_ignitionOnTimer.hasElapsedSec(config->ltftIgnitionOnDelay)) {
        return false;
    }

	// Outras condições já existentes (temperatura, etc)
	if (!config->ltftEnabled) return false;
	if ((Sensor::get(SensorType::Clt)).value_or(0) < float(config->ltftMinModTemp)) return false;
	return true;
}

void LongTermFuelTrim::updateLtft(float load, float rpm) {
	if (!canLearn()) return;

	// Verificar tempo mínimo entre atualizações (1 segundo)
	float updateIntervalSec = 1.0f;
	if (!m_updateTimer.hasElapsedSec(updateIntervalSec)) {
		return;
	}
	m_updateTimer.reset();

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
		// Aplicar correção bilinear apenas nas 4 células adjacentes
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

		// Após atualização da tabela, marcar aprendizado pendente
		updatedLtft = true;
	}
}

void LongTermFuelTrim::onIgnitionStateChanged(bool ignitionState) {
	m_ignitionState = ignitionState;

	if (ignitionState) {
		// Reset timers quando ignição liga
		m_ignitionOnTimer.reset();
		isLearnConditionsMet = false;
	} else if (updatedLtft) {
		// Reset timer para contagem do delay de salvamento
		m_ignitionOffTimer.reset();

		// Verificar se passou o tempo de delay após desligar a ignição
		float saveDelaySeconds = config->ltftIgnitionOffSaveDelay;
		if (saveDelaySeconds <= 0) {
			saveDelaySeconds = 5.0f; // Valor padrão de 5 segundos
		}

		// Na implementação atual, salvamos imediatamente
		// O ideal seria verificar o timer em um callback periódico
		copyTable(config->ltftTable, ltftTableHelper);
		setNeedToWriteConfiguration();
		updatedLtft = false;
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

	bool checkIfTuningVeNow() {
#if EFI_TUNER_STUDIO
		const bool result = isTuningVeNow();
#else
		const bool result = false;
#endif /* EFI_TUNER_STUDIO */
		engine->outputChannels.isTuningNow = result;
		return result;
	}

namespace {

	bool shouldCorrect() {
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
		ltftTableHelperInit = false;
		setNeedToWriteConfiguration();
	}

	if(!ltftTableHelperInit){
		copyTable(ltftTableHelper, config->ltftTable, 1);
		ltftTableHelperInit = true;
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
	m_ignitionOnTimer.reset();
	m_ignitionOffTimer.reset();
	m_updateTimer.reset();
	m_ignitionState = false;
	isLearnConditionsMet = false;
	ltftTableHelperInit = false;
}

#endif // EFI_ENGINE_CONTROL
